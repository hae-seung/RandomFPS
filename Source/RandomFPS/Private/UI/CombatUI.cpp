// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CombatUI.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/Components/PlayerCombatSystem.h"
#include "GameScene/Player/Components/PlayerWeapon.h"


void UCombatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TotalAmmoText->SetText(FText::AsNumber(0));
	MagAmmoText->SetText(FText::AsNumber(0));
	BulletSet->SetVisibility(ESlateVisibility::Hidden);

	HealthPreview->SetVisibility(ESlateVisibility::Hidden);
	
	CloseDeadUI();
}


void UCombatUI::Init(
	UPlayerCombatSystem* CombatComponent,
	UPlayerStatSystem* StatSystem,
	UPlayerWeapon* WeaponComponent)
{
	CombatComponent->OnReviveTimeChanged.AddUObject(this, &UCombatUI::UpdateReviveTime);
	CombatComponent->OnPlayerDead.AddUObject(this, &UCombatUI::OpenDeadUI);
	CombatComponent->OnPlayerRevive.AddUObject(this, &UCombatUI::CloseDeadUI);

	StatSystem->OnPlayerHealthStatChanged.AddUObject(this, &UCombatUI::UpdateHealthUI);
	StatSystem->OnPlayerEnergyChanged.AddUObject(this, &UCombatUI::UpdateEnergyUI);
	StatSystem->InitDelegates();

	//바인드는 총이 교체되는 최초 1회만 실행됨
	WeaponComponent->OnEquipGunActor.AddUObject(this, &UCombatUI::BindNewGunActorDelegates);
	WeaponComponent->OnChangeGunInstance.AddUObject(this, &UCombatUI::ChangeNewGunInstance);
}


#pragma region Gun
void UCombatUI::ChangeNewGunInstance(UGunItem* GunInstance)
{
	BulletSet->SetVisibility(ESlateVisibility::Visible);
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


void UCombatUI::BindNewGunActorDelegates(AGun* GunActor)
{
	GunActor->OnMagAmmoChanged.BindUObject(this, &UCombatUI::UpdateMagAmmoText);
	GunActor->OnTotalAmmoChanged.BindUObject(this, &UCombatUI::UpdateTotalAmmoText);
	GunActor->OnMagAmmoTypeChanged.BindUObject(this, &UCombatUI::UpdateMagAmmoTextColor);

	GunActor->InitDelegate();
}



#pragma endregion


#pragma region Combat

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


#pragma region Stat

void UCombatUI::UpdateHealthUI(const FPlayerHealthStat& Stat)
{
	const float Percent = Stat.Hp / Stat.MaxHp;
	
	HPBar->SetPercent(Percent);
	HealthPreview->SetPercent(Percent);

	if(Percent > 0.5f)
	{
		HPBar->SetFillColorAndOpacity(DefaultColor);
	}
	else if(Percent > 0.25f && Percent <= 0.5f)
	{
		HPBar->SetFillColorAndOpacity(OrangeColor);
	}
	else
	{
		HPBar->SetFillColorAndOpacity(RedColor);
	}
}

void UCombatUI::UpdateEnergyUI(int EnergyIndex)
{
	EnergyBar->SetPercent((EnergyIndex + 1) * 0.1f);
}


#pragma endregion