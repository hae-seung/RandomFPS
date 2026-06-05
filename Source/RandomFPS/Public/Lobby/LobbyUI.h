// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

class AMyPlayerState;
class UButton;
class UEditableTextBox;
class ULobbyGuestListItemUI;
class UWrapBox;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void RefreshLobbyGuests();

	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULobbyGuestListItemUI> BP_LobbyGuestItem;
	TMap<TObjectPtr<APlayerState>, TObjectPtr<ULobbyGuestListItemUI>> LobbyGuestItems;

	AMyPlayerState* MyPS;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* NickNameTxt;
	UPROPERTY(meta=(BindWidget))
	UButton* NickNameChangeBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* ReadyBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* StartBtn;
	UPROPERTY(meta=(BindWidget))
	UWrapBox* LobbyGuestsBox;
	

private:
	virtual void NativeOnInitialized() override;
	void SetBtns();
	
	UFUNCTION()
	void OnClickNickNameChanged();
	UFUNCTION()
	void OnReadyBtnClicked();
	UFUNCTION()
	void OnStartBtnClicked();

	void OnChangedRoomMembersReadyState(bool bIsAllReady);
};
