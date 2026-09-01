// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Cards/CardAbility/Base/MaxHpModifierAbility.h"

#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerStatSystem.h"


UCardAbility* UMaxHpModifierAbility::Clone()
{
	UMaxHpModifierAbility* NewCard = NewObject<UMaxHpModifierAbility>(this);
	NewCard->StatModifier = StatModifier;
	
	return NewCard;
}

void UMaxHpModifierAbility::OnAcquire(APlayerCharacter* Player)
{
	UPlayerStatSystem* StatSystem = Player->GetStatComponent();
	

	for(int i = 0; i < StatModifier.Num(); i++)
	{
		StatSystem->ApplyStatModifier(StatModifier[i]);
	}
}

void UMaxHpModifierAbility::OnRemove(APlayerCharacter* Player)
{
	
}
