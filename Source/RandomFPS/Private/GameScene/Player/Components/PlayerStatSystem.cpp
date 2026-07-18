// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/PlayerStatSystem.h"

#include "Net/UnrealNetwork.h"

UPlayerStatSystem::UPlayerStatSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UPlayerStatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerStatSystem, HealthStat);
	DOREPLIFETIME(UPlayerStatSystem, CombatStat);
	DOREPLIFETIME(UPlayerStatSystem, UtilityStat);
}


void UPlayerStatSystem::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		HealthStat.Hp = HealthStat.MaxHp;
	}

	InitDelegates();
}

void UPlayerStatSystem::InitDelegates() const
{
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
	OnPlayerCombatStatChanged.Broadcast(CombatStat);
	OnPlayerUtilityStatChanged.Broadcast(UtilityStat);
}

void UPlayerStatSystem::Revive()
{
	HealthStat.Hp = HealthStat.MaxHp;
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}


void UPlayerStatSystem::ModifyHp(float Delta)
{
	HealthStat.Hp = FMath::Clamp(HealthStat.Hp + Delta, 0, HealthStat.MaxHp);

	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}


void UPlayerStatSystem::OnRep_HealthStat()
{
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}

void UPlayerStatSystem::OnRep_CombatStat()
{
	
}

void UPlayerStatSystem::OnRep_UtilityStat()
{
	
}


