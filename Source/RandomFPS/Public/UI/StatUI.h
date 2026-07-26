// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/CombatStructHeader.h"
#include "StatUI.generated.h"

class UPlayerStatSystem;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UStatUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UPlayerStatSystem* PlayerStatSystem);


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

private:
	UPROPERTY()
	UPlayerStatSystem* StatSystem;

	FNumberFormattingOptions ZeroDigit;
	FNumberFormattingOptions OneDigit;
	
private:
	void UpdateHealthStat(const FPlayerHealthStat& HealthStat);
	void UpdateCombatStat(const FPlayerCombatStat& CombatStat);
	void UpdateUtilityStat(const FPlayerUtilityStat& UtilityStat);
};
