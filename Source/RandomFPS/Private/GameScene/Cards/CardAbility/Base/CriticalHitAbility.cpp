// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Cards/CardAbility/Base/CriticalHitAbility.h"

#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerCombatSystem.h"

UCardAbility* UCriticalHitAbility::Clone()
{
	UCriticalHitAbility* NewCard = NewObject<UCriticalHitAbility>(this);
	NewCard->PlusAttack = PlusAttack;
	
	return NewCard;
}

void UCriticalHitAbility::OnAcquire(APlayerCharacter* Player)
{
	UPlayerCombatSystem* CombatSystem = Player->GetCombatComponent();
	PlayerStatSystem = Player->GetStatComponent();
	
	CombatSystem->OnPlayerDead.AddUObject(this, &UCriticalHitAbility::ClearPlusAttack);
	CombatSystem->OnHitSuccess.AddUObject(this, &UCriticalHitAbility::HitCritical);
}

void UCriticalHitAbility::OnRemove(APlayerCharacter* Player)
{
	
}

void UCriticalHitAbility::ClearPlusAttack()
{
	PlayerStatSystem->ApplyStatModifier(FStatModifier(PlusAttack.Stat, -TotalPlusAmount));
}

void UCriticalHitAbility::HitCritical(bool bIsCritic)
{
	if(!bIsCritic)
		return;

	PlayerStatSystem->ApplyStatModifier(PlusAttack);
	TotalPlusAmount += PlusAttack.Value;
}
