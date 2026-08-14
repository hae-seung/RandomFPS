// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/CombatStructHeader.h"
#include "StatUI.generated.h"

class UReinforceStarUI;
class UHorizontalBox;
class UImage;
class UPlayerWeapon;
class UGunItem;
class UGunAbilityDescriptionUI;
class UVerticalBox;
class UPlayerStatSystem;
class UTextBlock;




UCLASS()
class RANDOMFPS_API UStatUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UPlayerStatSystem* PlayerStatSystem, UPlayerWeapon* PlayerWeapon);


private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxHpText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HpText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AttackDamageText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HeadShotDamageText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DefenseText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DefenseIgnoreRateText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HeadDamageIgnoreText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MonsterFlinchRateText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WalkSpeedText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ReviveTimeText;

	//=====================================//

	UPROPERTY(meta=(BindWidget))
	UImage* GunImage;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* DescriptionListBox;
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* StarBox;

private:
	UPROPERTY()
	UPlayerStatSystem* StatSystem;

	FNumberFormattingOptions ZeroDigit;
	FNumberFormattingOptions OneDigit;

	UPROPERTY()
	UGunItem* CurrentGun;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGunAbilityDescriptionUI> BP_DescriptionUI;
	UPROPERTY()
	TArray<TObjectPtr<UGunAbilityDescriptionUI>> GunDescriptions;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UReinforceStarUI> BP_StartUI;
	UPROPERTY()
	TArray<TObjectPtr<UReinforceStarUI>> Stars;

	
private:
	void UpdateHealthStat(const FPlayerHealthStat& HealthStat);
	void UpdateCombatStat(const FPlayerCombatStat& CombatStat);
	void UpdateUtilityStat(const FPlayerUtilityStat& UtilityStat);

	void ChangeGunInstance(UGunItem* GunItem);
	void UpdateGunAbility(int Level);
	void UpdateGunStar(int Level);
	void UpdateAwakeStar();
	void UpdateGunAwakeAbility();
};
