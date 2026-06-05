// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GunMenu.h"
#include "Components/Button.h"
#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/ItemData/GunItemData.h"
#include "UI/InventoryUI.h"


void UGunMenu::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("GunMenu Constructed %p"), this);
}

void UGunMenu::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp, Warning, TEXT("GunMenu Destructed %p"), this);
}


void UGunMenu::Init(UInventoryUI* InvenUI)
{
	InventoryUI = InvenUI;
	
	RifleBtn->OnClicked.AddDynamic(this, &UGunMenu::UGunMenu::OnClickRifleBtn);
	ShotgunBtn->OnClicked.AddDynamic(this, &UGunMenu::UGunMenu::OnClickShotgunBtn);
	SniperBtn->OnClicked.AddDynamic(this, &UGunMenu::UGunMenu::OnClickSniperBtn);
	
}

void UGunMenu::OnClickRifleBtn()
{
	HandleGunSelected(RifleItemData);
}

void UGunMenu::OnClickShotgunBtn()
{
	HandleGunSelected(ShotgunItemData);
}

void UGunMenu::OnClickSniperBtn()
{
	HandleGunSelected(SniperItemData);
}

void UGunMenu::HandleGunSelected(UGunItemData* GunItemData)
{
	if(!GunItemData || !InventoryUI->CheckInit())
	{
		return;
	}
	
	APlayerCharacter* Ch = GetOwningPlayerPawn<APlayerCharacter>();
	if(Ch)
	{
		RemoveFromParent();
		
		GetOwningPlayer<AMyPlayerController>()->SetInputModeGame();
		Ch->RequestAddItem(GunItemData, 1);
	}
}





