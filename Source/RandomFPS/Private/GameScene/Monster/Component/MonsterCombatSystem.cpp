// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/Component/MonsterCombatSystem.h"

#include "GameScene/Monster/MonsterData.h"
#include "Interface/Damageable.h"
#include "Net/UnrealNetwork.h"

UMonsterCombatSystem::UMonsterCombatSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UMonsterCombatSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UMonsterCombatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMonsterCombatSystem, MaxHp);
	DOREPLIFETIME(UMonsterCombatSystem, Hp);
}

void UMonsterCombatSystem::Init(UMonsterData* Data)
{
	Stat = Data->GetMonsterStatData();
	
	Hp = Stat.MaxHP;
	MaxHp = Stat.MaxHP;
	OnMonsterHealthStatChanged.Broadcast(Hp, MaxHp);
}

float UMonsterCombatSystem::TakeDamage(FDamageContext& Context)
{
	//데미지 공식
	//Damage×{K/(Armor+K)}
	const float FinalDamage = GetFinalDamage(Context);
	Hp = FMath::Clamp(Hp - FinalDamage, 0, Stat.MaxHP);
	
	//Server Feedback
	OnMonsterHealthStatChanged.Broadcast(Hp, MaxHp);
	
	if(Hp == 0)
	{
		//사망
		OnMonsterDead.Broadcast(true);

		//죽음 알림. 플레이어에게.
		
		return FinalDamage;
	}

	const int FlinchProb = Context.PlayerAttackerStat.MonsterFlinchProbability;
	if(FMath::RandRange(1,100) <= FlinchProb)
	{
		//flinch
		OnMonsterFlinched.Broadcast(Context.Attacker);
	}

	return FinalDamage;
}

float UMonsterCombatSystem::GetFinalDamage(FDamageContext& Context)
{
	float Damage = Context.FinalDamage;

	UE_LOG(LogTemp, Warning, TEXT("플레이어로부터 온 데미지 : %f"), Damage);
	//상대가 방어무시 가진 경우
	float EffectiveDefense = Stat.Defense;
	UE_LOG(LogTemp, Warning, TEXT("플레이어의 방무 스탯 : %f"), EffectiveDefense);
	
	EffectiveDefense *= (1.f - Context.PlayerAttackerStat.DismissDefenseRate);
	UE_LOG(LogTemp, Warning, TEXT("방무적용 후 몬스터의 방어력 : %f"), EffectiveDefense);
	
	
	Damage *= (Stat.K / (Stat.K + EffectiveDefense));
	UE_LOG(LogTemp, Warning, TEXT("몬스터에게 적용된 실데미지 : %f"), Damage);
	
	return Damage;
}



void UMonsterCombatSystem::ApplyDamage(FHitResult& HitResult)
{
	//몬스터 기준 맞은 놈은 무조건 플레이어임.(Trace로 걸러낸 상태임)
	if(IDamageable* DamageableActor = Cast<IDamageable>(HitResult.GetActor()))
	{
		FDamageContext AttackContext;
		AttackContext.EntityType = Stat.EntityType;
		AttackContext.Attacker = GetOwner();
		AttackContext.BaseDamage = Stat.AttackDamage;
		AttackContext.FinalDamage = CalculateAttackDamage(AttackContext);
		AttackContext.HitLocation = HitResult.Location;
		
		DamageableActor->TakeDamage(AttackContext);
	}
}

float UMonsterCombatSystem::CalculateAttackDamage(FDamageContext& AttackContext)
{
	bool IsCritical = FMath::RandRange(1, 100) <= Stat.CriticalProbability;
	AttackContext.bIsCritical = IsCritical;
	
	float Damage = IsCritical ? Stat.AttackDamage * 2 : Stat.AttackDamage;
	const float RandNum = Damage * Stat.RandomAttackDamageRange;
	Damage = FMath::RandRange(Damage - RandNum, Damage + RandNum);
	
	return  Damage;
}



void UMonsterCombatSystem::OnRep_Hp()
{
	if(Hp == MaxHp)
		return;

	//로컬의 몬스터 체력바 업데이트
	OnMonsterHealthStatChanged.Broadcast(Hp, MaxHp);
}

void UMonsterCombatSystem::OnRep_MaxHp()
{
	OnMonsterHealthStatChanged.Broadcast(Hp, MaxHp);
}
