// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/Weapon/Gun.h"
#include "Struct/CombatStructHeader.h"
#include "CombatUI.generated.h"

class UPlayerWeapon;
class UPlayerStatSystem;
class UKillLogEntryUI;
class UVerticalBox;
class UProgressBar;
class UOverlay;
class UImage;
class UPlayerCombatSystem;
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
	void Init(
		UPlayerCombatSystem* CombatComponent,
		UPlayerStatSystem* StatSystem,
		UPlayerWeapon* WeaponComponent);



private:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* BulletSet;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MagAmmoText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TotalAmmoText;

	UPROPERTY(meta=(BindWidget))
	UImage* DeathBackGround;
	UPROPERTY(meta=(BindWidget))
	UOverlay* ReviveInfo;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ReviveTimeText;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* EnergyBar;
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HPBar;
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthPreview;

	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor;
	UPROPERTY(EditAnywhere)
	FLinearColor OrangeColor;
	UPROPERTY(EditAnywhere)
	FLinearColor RedColor;

private:
	virtual void NativeOnInitialized() override;
	
	void UpdateTotalAmmoText(int TotalAmmo);
	void UpdateMagAmmoText(int MagAmmo);
	void UpdateMagAmmoTextColor(bool bIsRealBullet);
	
	void UpdateHealthUI(const FPlayerHealthStat& Stat);
	void UpdateEnergyUI(int EnergyIndex);
	
	void UpdateReviveTime(int ReviveRemainTime);
	void OpenDeadUI();
	void CloseDeadUI();

	void ChangeNewGunInstance(UGunItem* GunInstance);
	void BindNewGunActorDelegates(AGun* GunActor);
};
