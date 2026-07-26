// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatUI.h"

#include "Components/TextBlock.h"
#include "GameScene/Player/Components/PlayerStatSystem.h"

void UStatUI::Init(UPlayerStatSystem* PlayerStatSystem)
{
	StatSystem = PlayerStatSystem;
	
	ZeroDigit.MaximumFractionalDigits = 0;
	OneDigit.MinimumFractionalDigits = 1;
	OneDigit.MaximumFractionalDigits = 1;
	
	StatSystem->OnPlayerHealthStatChanged.AddUObject(this, &UStatUI::UpdateHealthStat);
	StatSystem->OnPlayerCombatStatChanged.AddUObject(this, &UStatUI::UpdateCombatStat);
	StatSystem->OnPlayerUtilityStatChanged.AddUObject(this, &UStatUI::UpdateUtilityStat);

	StatSystem->InitDelegates();
}

void UStatUI::UpdateHealthStat(const FPlayerHealthStat& HealthStat)
{
	MaxHpText->SetText(FText::AsNumber(HealthStat.MaxHp, &ZeroDigit));
	HpText->SetText(FText::AsNumber(HealthStat.Hp, &ZeroDigit));
}

void UStatUI::UpdateCombatStat(const FPlayerCombatStat& CombatStat)
{
	AttackDamageText->SetText(FText::AsNumber(CombatStat.AttackDamage, &ZeroDigit));
	
	HeadShotDamageText->SetText(
		FText::Format(
		FText::FromString(TEXT("X {0}")),
		FText::AsNumber(CombatStat.CriticalDamageBonus, &OneDigit)
	));
	
	DefenseText->SetText(FText::AsNumber(CombatStat.Defense, &ZeroDigit));
	
	DefenseIgnoreRateText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}%")),
			FText::AsNumber(CombatStat.DismissDefenseRate * 100, &ZeroDigit)
			));
	
	HeadDamageIgnoreText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}%")),
			FText::AsNumber(CombatStat.CriticalDamageDefense * 100, &ZeroDigit)
			));
	
	MonsterFlinchRateText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}%")),
			FText::AsNumber(CombatStat.MonsterFlinchProbability, &ZeroDigit)
			));
}

void UStatUI::UpdateUtilityStat(const FPlayerUtilityStat& UtilityStat)
{
	UE_LOG(LogTemp, Warning, TEXT("UI WalkSpeed = %f"), UtilityStat.WalkSpeed);
	WalkSpeedText->SetText(FText::AsNumber(UtilityStat.WalkSpeed, &ZeroDigit));
	ReviveTimeText->SetText(
		FText::Format(
		FText::FromString(TEXT("{0}초")),
		FText::AsNumber(UtilityStat.ReviveTime, &ZeroDigit)));
}


