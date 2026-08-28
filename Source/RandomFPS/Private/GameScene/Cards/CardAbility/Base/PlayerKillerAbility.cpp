// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Cards/CardAbility/Base/PlayerKillerAbility.h"

#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerStatSystem.h"

UCardAbility* UPlayerKillerAbility::Clone()
{
	return NewObject<UPlayerKillerAbility>(this);
}

void UPlayerKillerAbility::OnAcquire(APlayerCharacter* Player)
{
	PlayerStatSystem = Player->GetStatComponent();
	Player->KillPlayerEvent.AddUObject(this, &UPlayerKillerAbility::KillPlayer);
}

void UPlayerKillerAbility::OnRemove(APlayerCharacter* Player)
{
	
}

void UPlayerKillerAbility::KillPlayer(AActor* A, AActor* B, bool bCritic)
{
	for(int i = 0; i < Modifiers.Num(); i++)
	{
		PlayerStatSystem->ApplyStatModifier(Modifiers[i]);
	}
}

