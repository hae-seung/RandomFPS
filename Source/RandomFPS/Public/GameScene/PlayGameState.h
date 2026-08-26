// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Struct/RoundStruct.h"
#include "PlayGameState.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(OnPlayerStateAdd, APlayerState*);
DECLARE_MULTICAST_DELEGATE_OneParam(OnPlayerStateRemoved, APlayerState*)

DECLARE_MULTICAST_DELEGATE_ThreeParams(OnKillLogShow, APlayerState*, APlayerState*, bool);

DECLARE_MULTICAST_DELEGATE_OneParam(OnRoundTimeChanged, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(OnRoundInfoChanged, int, const FRoundInfo&);


UCLASS()
class RANDOMFPS_API APlayGameState : public AGameState
{
	GENERATED_BODY()

public:
	OnPlayerStateAdd PlayerStateAdd;
	OnPlayerStateRemoved PlayerStateRemoved;

	OnKillLogShow KillLogShowEvent;

	OnRoundTimeChanged RoundTimeChanged;
	OnRoundInfoChanged RoundInfoChanged;

	
public:
	void SetRoundTimer(float ServerEndTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetRoundInfo(int CurrentRound, FRoundInfo RoundInfo);
	void SetRoundInfo(int CurrentRound, const FRoundInfo& RoundInfo);
	void SetPlayerShotAvailable(bool bState);
	void GiveReward(const FRoundInfo& RoundInfo);
	
	void Server_GetPlayerKillEvent(AActor* Killer, AActor* Victim, bool bIsCriticalKill);
	
	
	
	const FRoundInfo* GetCurrentRoundInfo();
	ERoundType GetRoundType() const;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;


private:
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_RoundEndTime)
	float RoundEndTime;
	UPROPERTY(Replicated)
	ERoundType RoundType;
	
private:
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_GetPlayerKillEvent(APlayerState* KillerState, APlayerState* VictimState, bool bIsCritical);
	UFUNCTION()
	void OnRep_RoundEndTime();


	void RecoveryAllPlayerLife(int PlusLife);
	void GiveAllPlayerGold(int PlusGold);
	void GiveAllPlayerCard();
};
