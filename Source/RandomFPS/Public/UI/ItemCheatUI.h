// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemCheatUI.generated.h"


class AMyPlayerController;
class APlayerCharacter;
class UButton;
class UItemData;
class UCheatButton;

UCLASS()
class RANDOMFPS_API UItemCheatUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(APlayerCharacter* InAPC, AMyPlayerController* MPC);
	void Toggle();
	
protected:
	virtual void NativeOnInitialized() override;


private:
	UPROPERTY(meta=(BindWidget))
	UCheatButton* RedDotBtn;
	UPROPERTY(meta=(BindWidget))
	UCheatButton* ScopeBtn;
	UPROPERTY(meta=(BindWidget))
	UCheatButton* HoloBtn;
	UPROPERTY(meta=(BindWidget))
	UCheatButton* MagazineBtn;
	UPROPERTY(meta=(BindWidget))
	UCheatButton* SuppressorBtn;
	UPROPERTY(meta=(BindWidget))
	UCheatButton* CompensatorBtn;
	UPROPERTY(meta=(BindWidget))
	UCheatButton* BulletBtn;
	UPROPERTY(meta=(BindWidget))
	UCheatButton* PortionBtn;

	
	UPROPERTY(meta=(BindWidget))
	UButton* LevelUpBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* AwakeBtn;


private:
	UPROPERTY()
	APlayerCharacter* APC;
	UPROPERTY()
	AMyPlayerController* MyController;
	
	bool bIsOpen;
	
private:
	void GiveItem(UItemData* ItemData);

	UFUNCTION()
	void LevelUP();
	UFUNCTION()
	void Awake();
};
