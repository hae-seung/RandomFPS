// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/PlayGameState.h"

#include "GameScene/Player/MyPlayerState.h"
#include "GameScene/Player/PlayerCharacter.h"


void APlayGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	PlayerStateAdd.Broadcast(PlayerState);
}

void APlayGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	PlayerStateRemoved.Broadcast(PlayerState);
}


void APlayGameState::Server_GetPlayerKillEvent(AActor* Killer, AActor* Victim, bool bIsCriticalKill)
{
	AMyPlayerState* APS = nullptr;
	AMyPlayerState* BPS = nullptr;
	
	if(APlayerCharacter* A = Cast<APlayerCharacter>(Killer))
	{
		APS = A->GetPlayerState<AMyPlayerState>();
	}

	if(APlayerCharacter* B = Cast<APlayerCharacter>(Victim))
	{
		BPS = B->GetPlayerState<AMyPlayerState>();
	}

	if(IsValid(APS) && IsValid(BPS))
	{
		KillLogShowEvent.Broadcast(APS, BPS, bIsCriticalKill);
		NetMulticast_GetPlayerKillEvent(APS, BPS, bIsCriticalKill);
	}
}

void APlayGameState::NetMulticast_GetPlayerKillEvent_Implementation(APlayerState* KillerState, APlayerState* VictimState, bool bIsCritical)
{
	if(HasAuthority())
		return;

	KillLogShowEvent.Broadcast(KillerState, VictimState, bIsCritical);
}

