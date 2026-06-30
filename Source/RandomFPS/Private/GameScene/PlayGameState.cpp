// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/PlayGameState.h"


void APlayGameState::Server_GetPlayerKillEvent(AActor* Killer, AActor* Victim)
{
}

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
