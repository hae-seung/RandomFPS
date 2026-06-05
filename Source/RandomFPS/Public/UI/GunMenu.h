// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GunMenu.generated.h"

class UGunItemData;
class UButton;
class UInventoryUI;


UCLASS()
class RANDOMFPS_API UGunMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UInventoryUI* InvenUI);
	
private:
	UPROPERTY(EditAnywhere)
	UGunItemData* RifleItemData;
	UPROPERTY(EditAnywhere)
	UGunItemData* ShotgunItemData;
	UPROPERTY(EditAnywhere)
	UGunItemData* SniperItemData;

	
	UPROPERTY(meta=(AllowPrivateAccess, BindWidget))
	UButton* RifleBtn;
	UPROPERTY(meta=(AllowPrivateAccess, BindWidget))
	UButton* ShotgunBtn;
	UPROPERTY(meta=(AllowPrivateAccess, BindWidget))
	UButton* SniperBtn;
	
	UInventoryUI* InventoryUI;


private:
	UFUNCTION()
	void OnClickRifleBtn();
	UFUNCTION()
	void OnClickShotgunBtn();
	UFUNCTION()
	void OnClickSniperBtn();
	
	void HandleGunSelected(UGunItemData* GunItemData);
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
