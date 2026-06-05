// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"

class USearchRoomUI;
class UCreateSessionUI;
class UButton;
class UOverlay;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetBtnInteract(bool bInteract)const;

private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UOverlay* MenuBtnSet;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UOverlay* PlayBtnSet;

	UPROPERTY(meta=(BindWidget))
	UButton* StartBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* SettingBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* ExitBtn;

	UPROPERTY(meta=(BindWidget))
	UButton* HostBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* JoinBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* BackBtn;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UCreateSessionUI* CreateSessionUI;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	USearchRoomUI* SearchRoomUI;

	

private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnClickedStartBtn();
	UFUNCTION()
	void OnClickedSettingBtn();
	UFUNCTION()
	void OnClickedExitBtn();

	UFUNCTION()
	void OnClickedHostBtn();
	UFUNCTION()
	void OnClickedJoinBtn();
	UFUNCTION()
	void OnClickedBackBtn();

	UFUNCTION()
	void OnCreateSessionFail();
};
