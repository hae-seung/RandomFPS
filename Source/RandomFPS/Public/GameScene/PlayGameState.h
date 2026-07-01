// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PlayGameState.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(OnPlayerStateAdd, APlayerState*);
DECLARE_MULTICAST_DELEGATE_OneParam(OnPlayerStateRemoved, APlayerState*)

DECLARE_MULTICAST_DELEGATE_ThreeParams(OnKillLogShow, APlayerState*, APlayerState*, bool);


UCLASS()
class RANDOMFPS_API APlayGameState : public AGameState
{
	GENERATED_BODY()

public:
	OnPlayerStateAdd PlayerStateAdd;
	OnPlayerStateRemoved PlayerStateRemoved;

	OnKillLogShow KillLogShowEvent;

public:
	void Server_GetPlayerKillEvent(AActor* Killer, AActor* Victim, bool bIsCriticalKill);
	
protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

private:
	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_GetPlayerKillEvent(APlayerState* KillerState, APlayerState* VictimState, bool bIsCritical);
};
