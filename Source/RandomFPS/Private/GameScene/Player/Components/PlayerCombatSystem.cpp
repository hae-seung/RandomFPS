// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/PlayerCombatSystem.h"

#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"


UPlayerCombatSystem::UPlayerCombatSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UPlayerCombatSystem::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetOwner()->HasAuthority())
	{
		HealthStat.Hp = HealthStat.MaxHp;
	}
	
	//multi 순서문제
	SubScribeInit();
}

void UPlayerCombatSystem::SubScribeInit()
{
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
	OnPlayerCombatStatChanged.Broadcast(CombatStat);
	OnPlayerUtilityStatChanged.Broadcast(UtilityStat);
}


void UPlayerCombatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerCombatSystem, HealthStat);
	DOREPLIFETIME(UPlayerCombatSystem, CombatStat);
	DOREPLIFETIME(UPlayerCombatSystem, UtilityStat);
	DOREPLIFETIME(UPlayerCombatSystem, RemainReviveTime);
}

void UPlayerCombatSystem::ApplyDamageToTarget(
	IDamageable* Target,
	FVector HitLocation,
	FName BoneName,
	bool bIsRealBullet)
{
	const bool bIsCritical = BoneName == "head";
	
	FDamageContext Context;
	Context.Attacker = GetOwner();
	Context.EntityType = EEntityType::Player;
	Context.HitLocation = HitLocation;
	Context.bIsCritical = bIsCritical;

	float Damage = CombatStat.AttackDamage;
	if(!bIsRealBullet)
	{
		Damage *= 0.5f;
	}
	Context.BaseDamage = Damage;
	Context.FinalDamage = CalculateAttackDamage(Damage, bIsCritical);
	Context.PlayerAttackerStat = CombatStat;

	Target->TakeDamage(Context);
	OnHitSuccess.Broadcast(bIsCritical);
	Multicast_HitSuccess(bIsCritical);
}

float UPlayerCombatSystem::CalculateAttackDamage(float Damage, bool bIsCritic)
{
	//치명타보너스는 현재 데미지의 몇배인지
	float FinalDamage = bIsCritic ? Damage * CombatStat.CriticalDamageBonus : Damage;

	//데미지 랜덤 난수 : 기본 : 1% = 0.01
	const float RandNum = FinalDamage * CombatStat.DamageRand;
	FinalDamage = FMath::RandRange(FinalDamage - RandNum, FinalDamage + RandNum);
	
	return  FinalDamage;
}

//server
void UPlayerCombatSystem::TakeDamage(FDamageContext& Context)
{
	const int FinalGetDamage = CalculateGetDamage(Context);
	if(Context.EntityType == EEntityType::Player)
	{
		HitMePlayers.Add(Context.Attacker, GetWorld()->GetTimeSeconds());
	}
	
	HealthStat.Hp = FMath::Max(0, HealthStat.Hp - FinalGetDamage);
	if(HealthStat.Hp <= 0)
	{
		//die
		Dead(Context.Attacker, Context.bIsCritical);
		StartReviveTimer();
	}
	
	//UI
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}

int UPlayerCombatSystem::CalculateGetDamage(FDamageContext& Context)
{
	float TotalDamage = Context.BaseDamage;

	if (Context.bIsCritical)
	{
		float CritBonus = Context.FinalDamage - Context.BaseDamage;
		CritBonus *= (1.f - CombatStat.CriticalDamageDefense); //치명타 데미지 감소 스탯 적용
		TotalDamage += CritBonus;
	}

	float EffectiveDefense = CombatStat.Defense;

	//상대 플레이어가 방어율 무시 스탯을 가진 경우
	if (Context.EntityType == EEntityType::Player)
	{
		EffectiveDefense *= (1.f - Context.PlayerAttackerStat.DismissDefenseRate);
	}

	TotalDamage *= (CombatStat.K / (CombatStat.K + EffectiveDefense));

	return (int)(TotalDamage);
}

void UPlayerCombatSystem::StartReviveTimer()
{
	RemainReviveTime = UtilityStat.ReviveTime; //replicated
	OnReviveTimeChanged.Broadcast(RemainReviveTime);
	
	GetWorld()->GetTimerManager().SetTimer(
		ReviveTimer,
		this, &UPlayerCombatSystem::CheckReviveTime,
		1.f,
		true);
}

//server
void UPlayerCombatSystem::CheckReviveTime()
{
	RemainReviveTime -= 1; //replicated
	OnReviveTimeChanged.Broadcast(RemainReviveTime);
	
	if(RemainReviveTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReviveTimer);
		
		OnPlayerRevive.Broadcast();
		
		//부활시 체력 회복
		HealthStat.Hp = HealthStat.MaxHp;
		OnPlayerHealthStatChanged.Broadcast(HealthStat);
	}
}


void UPlayerCombatSystem::Dead(AActor* Attacker, bool bIsCritical)
{
	OnPlayerDead.Broadcast();
	
	//날 죽인놈에게 니가 킬 했다고 알려줌
	if(IKillable* Killable = Cast<IKillable>(Attacker))
	{
		Killable->KillOtherPlayer(GetOwner(), bIsCritical);

		if(HitMePlayers.Contains(Attacker))
		{
			HitMePlayers.Remove(Attacker);
		}
	}
	else
	{
		return;
	}

	//assist
	//반복문으로 나를 때린 사람들에게 전부 IKillable의 Assist함수 호출 this를 넘김.
	float DeadTime = GetWorld()->GetTimeSeconds();
	for(const auto& Pair : HitMePlayers)
	{
		if(DeadTime - Pair.Value <= AssistTime)
		{
			if(IKillable* Killable = Cast<IKillable>(Pair.Key))
			{
				Killable->GetAssist(GetOwner());
			}
		}
	}

	HitMePlayers.Reset();
}


void UPlayerCombatSystem::OnRep_HealthStat()
{
	if(HealthStat.Hp <= 0)
	{
		OnPlayerDead.Broadcast();
	}

	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}

void UPlayerCombatSystem::OnRep_CombatStat()
{
	
}

void UPlayerCombatSystem::OnRep_UtilityStat()
{
	
}



void UPlayerCombatSystem::OnRep_RemainReviveTime()
{
	OnReviveTimeChanged.Broadcast(RemainReviveTime);

	if(RemainReviveTime <= 0)
	{
		OnPlayerRevive.Broadcast();
	}
}


void UPlayerCombatSystem::Multicast_HitSuccess_Implementation(bool bIsCritical)
{
	if(GetOwner()->HasAuthority())
		return;

	OnHitSuccess.Broadcast(bIsCritical);
}
