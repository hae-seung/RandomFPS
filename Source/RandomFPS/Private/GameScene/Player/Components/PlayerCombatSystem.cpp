// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/PlayerCombatSystem.h"

#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Player/PlayerCharacter.h"


UPlayerCombatSystem::UPlayerCombatSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerCombatSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerCombatSystem::TakeDamage(FDamageContext& Context)
{
	const int FinalGetDamage = CalculateGetDamage(Context);
	Hp -= FinalGetDamage;
	if(Hp <= 0)
	{
		//die
		APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner());
		if(APC)
		{
			APC->Dead();
		}
		return;
	}

	//UI
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