// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyController.generated.h"

class ULobbyUI;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API ALobbyController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULobbyUI> BP_LobbyUI;
	ULobbyUI* LobbyUI;


private:
	virtual void BeginPlay() override;

	//클라이언트쪽에서 복제된 플레이어스테이트를 수신했음을 알림받음.
	virtual void OnRep_PlayerState() override;
};
