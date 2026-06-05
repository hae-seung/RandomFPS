// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyGuestListItemUI.generated.h"

class AMyPlayerState;
class USizeBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API ULobbyGuestListItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE AMyPlayerState* GetPlayerState()const { return CachedPlayerState; }
	void Init(AMyPlayerState* PS);
	void Refresh();
	
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerNameTxt;
	UPROPERTY(meta=(BindWidget))
	USizeBox* ReadySet; //readyImage
	UPROPERTY(meta=(BindWidget))
	USizeBox* LobbyMasterImage;

	
	AMyPlayerState* CachedPlayerState;

private:
	void CheckIsPlayerMaster();
	void OnNickNameChangedReceive();
	void OnReadyStateChangedReceive();
};
