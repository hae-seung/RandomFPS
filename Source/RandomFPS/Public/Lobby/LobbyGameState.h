// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

DECLARE_DELEGATE(FOnPlayerListChangedEvent);
DECLARE_DELEGATE_OneParam(FOnReadyPlayerCntChangedEvent, bool);



UCLASS()
class RANDOMFPS_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()

public:
	FOnPlayerListChangedEvent OnPlayerListChangedEvent;
	FOnReadyPlayerCntChangedEvent OnReadyPlayerCntChangedEvent;

	
public:
	void SetReadyState(bool bIsReady);
	

private:
	UPROPERTY(Replicated)
	int32 CurrentRoomMemberCnt = 0;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_ReadyPlayerCnt)
	int32 ReadyPlayerCnt = 0;

	
private:
	//새로운 플레이어가 서버에 들어가거나 나가는 경우를 위함//
	
	//서버에서 플레이어Array가 변경되면 renotify됨. => 클라이언트들만 수신
	UFUNCTION()
	void OnRep_PlayerArray();

	//서버에서만 호출됨. =>서버 전용
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	void HandlePlayerListChanged();

	UFUNCTION()
	void OnRep_ReadyPlayerCnt();
	void SetRoomMasterStartBtn();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
