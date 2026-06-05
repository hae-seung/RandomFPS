// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CombatUI.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"


void UCombatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
	TotalAmmoText->SetText(FText::AsNumber(0));
	MagAmmoText->SetText(FText::AsNumber(0));
	BulletSet->SetVisibility(ESlateVisibility::Hidden);
}


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
