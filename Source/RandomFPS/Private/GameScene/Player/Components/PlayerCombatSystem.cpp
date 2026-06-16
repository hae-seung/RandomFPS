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

//server
void UPlayerCombatSystem::TakeDamage(FDamageContext& Context)
{
	const int FinalGetDamage = CalculateGetDamage(Context);
	HealthStat.Hp = FMath::Max(0, HealthStat.Hp - FinalGetDamage);
	if(HealthStat.Hp <= 0)
	{
		//die
		OnPlayerDead.Broadcast();
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
		CritBonus *= (1.f - CombatStat.CriticalDamageDefense);
		TotalDamage += CritBonus;
	}

	float EffectiveDefense = CombatStat.Defense;

	if (Context.EntityType == EEntityType::Player)
	{
		EffectiveDefense *= (1.f - Context.PlayerContext.PlayerCombatStat->DismissDefenseRate);
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
