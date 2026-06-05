// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionUI.generated.h"


class UMainMenuUI;
class UButton;
class UCheckBox;
class UEditableText;

UCLASS()
class RANDOMFPS_API UCreateSessionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 LimitMaxPlayerCount = 6;

	void Init(UMainMenuUI* MainMenu);
	
private:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess))
	UEditableText* EditNameTxt;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess))
	UEditableText* EditNumTxt;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess))
	UButton* CreateBtn;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess))
	UButton* CloseBtn;


	UMainMenuUI* MainMenuUI;
	
	bool bIsLanMode = false;
	FString RoomName;
	int32 MaxPlayerCount = 0;
	
private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnEditNameTxtChanged(const FText& Text);
	UFUNCTION()
	void OnEditNumTxtChanged(const FText& Text);
	UFUNCTION()
	void OnCreateBtnClicked();
	UFUNCTION()
	void OnCloseBtnClicked();
};
