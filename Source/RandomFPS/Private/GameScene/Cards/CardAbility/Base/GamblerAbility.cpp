// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Cards/CardAbility/Base/GamblerAbility.h"

#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerWalletSystem.h"

UCardAbility* UGamblerAbility::Clone()
{
	return NewObject<UGamblerAbility>(this);
}

void UGamblerAbility::OnAcquire(APlayerCharacter* Player)
{
	PlayerWallet = Player->GetWalletSystem();
	Player->KillPlayerEvent.AddUObject(this, &UGamblerAbility::KillOtherPlayer);
}

void UGamblerAbility::OnRemove(APlayerCharacter* Player)
{
	
}

void UGamblerAbility::KillOtherPlayer(AActor* A, AActor* B, bool bIsCritic)
{
	int Gold = FMath::RandRange(MinGold, MaxGold);
	PlayerWallet->AcquireMoney(Gold);
}




