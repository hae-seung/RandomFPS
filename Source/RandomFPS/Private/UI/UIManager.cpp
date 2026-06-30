// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIManager.h"
#include "GameFramework/PlayerState.h"
#include "Components/Button.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/ItemData/BulletItemData.h"
#include "GameScene/Player/ItemData/PartsData/RailPartsData.h"
#include "UI/CombatUI.h"
#include "UI/CrossHairUI.h"
#include "UI/GunMenu.h"
#include "UI/InventoryUI.h"
#include "UI/ScoreUI.h"


void UUIManager::Init(APawn* Pawn)
{
	APlayerCharacter* APC = Cast<APlayerCharacter>(Pawn);
	
	InventoryUI->Init();
	GunMenuUI->Init(InventoryUI);
	CombatUI->Init(APC->GetCombatComponent());
	CrossHairUI->Init(APC->GetCombatComponent());
	ScoreUI->FindLocalEntry(GetOwningPlayerState());

	RedDotBtn->OnClicked.AddDynamic(this, &UUIManager::GiveRedDot);
	BulletBtn->OnClicked.AddDynamic(this, &UUIManager::GiveBullet);
}

void UUIManager::GiveRedDot()
{
	APlayerCharacter* APC = GetOwningPlayer()->GetPawn<APlayerCharacter>();
	if(APC)
	{
		APC->RequestAddItem(RailData);
	}
}

void UUIManager::GiveBullet()
{
	APlayerCharacter* APC = GetOwningPlayer()->GetPawn<APlayerCharacter>();
	if(APC)
	{
		APC->RequestAddItem(BulletService.BulletItemData, BulletService.Amount);
	}
}


void UUIManager::ToggleInventory()
{
	InventoryUI->Toggle();
}

void UUIManager::OpenScoreBoard()
{
	ScoreUI->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUIManager::CloseScoreBoard()
{
	ScoreUI->SetVisibility(ESlateVisibility::Collapsed);
}

