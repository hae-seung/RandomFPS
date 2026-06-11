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
		Stat.Hp = Stat.MaxHP;
	}
	
	//multi 순서문제
	SubScribeInit();
}

void UPlayerCombatSystem::SubScribeInit()
{
	OnStateChanged.Broadcast(Stat);
}


void UPlayerCombatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerCombatSystem, Stat);
	DOREPLIFETIME(UPlayerCombatSystem, RemainReviveTime);
}

//server
void UPlayerCombatSystem::TakeDamage(FDamageContext& Context)
{
	const int FinalGetDamage = CalculateGetDamage(Context);
	Stat.Hp = FMath::Max(0, Stat.Hp - FinalGetDamage);
	if(Stat.Hp <= 0)
	{
		//die
		OnPlayerDead.Broadcast();
		StartReviveTimer();
	}
	
	//UI
	OnStateChanged.Broadcast(Stat);
}


int UPlayerCombatSystem::CalculateGetDamage(FDamageContext& Context)
{
	float TotalDamage = Context.BaseDamage;

	if (Context.bIsCritical)
	{
		float CritBonus = Context.FinalDamage - Context.BaseDamage;
		CritBonus *= (1.f - Stat.CriticalDamageDefense);
		TotalDamage += CritBonus;
	}

	float EffectiveDefense = Stat.Defense;

	if (Context.EntityType == EEntityType::Player)
	{
		EffectiveDefense *= (1.f - Context.PlayerContext.PlayerStat->DismissDefenseRate);
	}

	TotalDamage *= (Stat.K / (Stat.K + EffectiveDefense));

	return (int)(TotalDamage);
}

void UPlayerCombatSystem::StartReviveTimer()
{
	RemainReviveTime = Stat.ReviveTime; //replicated
	OnReviveTimeChanged.Broadcast(RemainReviveTime);
	
	GetWorld()->GetTimerManager().SetTimer(
		ReviveTimer,
		this, &UPlayerCombatSystem::CheckReviveTime,
		1.f,
		true);
}

void UPlayerCombatSystem::CheckReviveTime()
{
	RemainReviveTime -= 1; //replicated
	OnReviveTimeChanged.Broadcast(RemainReviveTime);
	
	if(RemainReviveTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReviveTimer);
		
		OnPlayerRevive.Broadcast();
		
		//부활시 체력 회복
		Stat.Hp = Stat.MaxHP;
		OnStateChanged.Broadcast(Stat);
	}
}

void UPlayerCombatSystem::OnRep_Stat()
{
	if(Stat.Hp <= 0)
	{
		OnPlayerDead.Broadcast();
	}
	
	OnStateChanged.Broadcast(Stat);
}

void UPlayerCombatSystem::OnRep_RemainReviveTime()
{
	OnReviveTimeChanged.Broadcast(RemainReviveTime);

	if(RemainReviveTime <= 0)
	{
		OnPlayerRevive.Broadcast();
	}
}
