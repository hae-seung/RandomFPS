// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CombatUI.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/Components/PlayerCombatSystem.h"


void UCombatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
	TotalAmmoText->SetText(FText::AsNumber(0));
	MagAmmoText->SetText(FText::AsNumber(0));
	BulletSet->SetVisibility(ESlateVisibility::Hidden);

	HealthPreview->SetVisibility(ESlateVisibility::Hidden);
	
	CloseDeadUI();
}


void UCombatUI::Init(UPlayerCombatSystem* CombatComponent)
{
	CombatComponent->OnPlayerHealthStatChanged.AddUObject(this, &UCombatUI::UpdateHealthUI);
	CombatComponent->OnReviveTimeChanged.AddUObject(this, &UCombatUI::UpdateReviveTime);
	CombatComponent->OnPlayerDead.AddUObject(this, &UCombatUI::OpenDeadUI);
	CombatComponent->OnPlayerRevive.AddUObject(this, &UCombatUI::CloseDeadUI);

	CombatComponent->SubScribeInit();
}


#pragma region Gun
void UCombatUI::EquipGun()
{
	BulletSet->SetVisibility(ESlateVisibility::Visible);
}

void UCombatUI::BindTotalAmmoDelegate(FOnTotalAmmoChanged& OnTotalAmmoChanged)
{
	OnTotalAmmoChanged.BindUObject(this, &UCombatUI::UpdateTotalAmmoText);
}

void UCombatUI::BindMagAmmoDelegate(FOnMagAmmoChanged& OnMagAmmoChanged)
{
	OnMagAmmoChanged.BindUObject(this, &UCombatUI::UpdateMagAmmoText);
}

void UCombatUI::BindMagAmmoTypeDelegate(FOnMagAmmoTypeChanged& OnMagAmmoTypeChanged)
{
	OnMagAmmoTypeChanged.BindUObject(this, &UCombatUI::UpdateMagAmmoTextColor);
}

void UCombatUI::UpdateTotalAmmoText(int TotalAmmo)
{
	if(TotalAmmo > 0)
	{
		TotalAmmoText->SetText(
		FText::FromString(FString::Printf(TEXT(" | %d"), TotalAmmo)));
	}
	else
	{
		TotalAmmoText->SetText(FText::FromString(TEXT("| ∞")));
	}
}

void UCombatUI::UpdateMagAmmoText(int MagAmmo)
{
	MagAmmoText->SetText(FText::AsNumber(MagAmmo));
}

void UCombatUI::UpdateMagAmmoTextColor(bool bIsRealBullet)
{
	MagAmmoText->SetColorAndOpacity(
		bIsRealBullet ? FLinearColor::White : FLinearColor::Red);
}


#pragma endregion


#pragma region Combat

void UCombatUI::UpdateHealthUI(const FPlayerHealthStat& Stat)
{
	const float Percent = Stat.Hp / Stat.MaxHp;
	
	HPBar->SetPercent(Percent);
	HealthPreview->SetPercent(Percent);
}

void UCombatUI::UpdateReviveTime(int ReviveRemainTime)
{
	ReviveTimeText->SetText(FText::AsNumber(ReviveRemainTime));
}

void UCombatUI::OpenDeadUI()
{
	DeathBackGround->SetVisibility(ESlateVisibility::Visible);
	ReviveInfo->SetVisibility(ESlateVisibility::Visible);
}

void UCombatUI::CloseDeadUI()
{
	DeathBackGround->SetVisibility(ESlateVisibility::Collapsed);
	ReviveInfo->SetVisibility(ESlateVisibility::Collapsed);
}

#pragma endregion