// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/Weapon/Gun.h"
#include "CombatUI.generated.h"

class UTextBlock;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UCombatUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void EquipGun();
	
	void BindTotalAmmoDelegate(FOnTotalAmmoChanged& OnTotalAmmoChanged);
	void BindMagAmmoDelegate(FOnMagAmmoChanged& OnMagAmmoChanged);
	void BindMagAmmoTypeDelegate(FOnMagAmmoTypeChanged& OnMagAmmoTypeChanged);
	
	
private:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* BulletSet;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MagAmmoText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TotalAmmoText;

private:
	virtual void NativeOnInitialized() override;
	
	void UpdateTotalAmmoText(int TotalAmmo);
	void UpdateMagAmmoText(int MagAmmo);
	void UpdateMagAmmoTextColor(bool bIsRealBullet);
};
