// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIManager.h"
#include "GameFramework/PlayerState.h"
#include "Components/Button.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerWeapon.h"
#include "GameScene/Player/ItemData/BulletItemData.h"
#include "GameScene/Player/ItemData/PortionItemData.h"
#include "GameScene/Player/ItemData/PartsData/RailPartsData.h"
#include "UI/CombatUI.h"
#include "UI/CrossHairUI.h"
#include "UI/GunMenu.h"
#include "UI/InteractorUI.h"
#include "UI/InventoryUI.h"
#include "UI/KillLogUI.h"
#include "UI/ScoreUI.h"


void UUIManager::Init(APawn* Pawn)
{
	APlayerCharacter* APC = Cast<APlayerCharacter>(Pawn);
	
	InventoryUI->Init(APC->GetInventory(), APC->GetStatComponent(), APC->GetWeaponSystem());
	GunMenuUI->Init(InventoryUI);
	CombatUI->Init(APC->GetCombatComponent(), APC->GetStatComponent(), APC->GetWeaponSystem());
	CrossHairUI->Init(APC->GetCombatComponent());
	ScoreUI->FindLocalEntry(GetOwningPlayerState());
	KillLogUI->Init(APC->KillAlarmEvent, APC->AssistAlarmEvent);
	InteractorUI->Init(APC->GetInteractSystem());

	
	RedDotBtn->OnClicked.AddDynamic(this, &UUIManager::GiveRedDot);
	BulletBtn->OnClicked.AddDynamic(this, &UUIManager::GiveBullet);
	PortionBtn->OnClicked.AddDynamic(this,&UUIManager::GivePortion);
	LevelUpBtn->OnClicked.AddDynamic(this, &UUIManager::LevelUpGun);
	AwakeBtn->OnClicked.AddDynamic(this, &UUIManager::AwakeGun);
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

void UUIManager::GivePortion()
{
	APlayerCharacter* APC = GetOwningPlayer()->GetPawn<APlayerCharacter>();
	if(APC)
	{
		APC->RequestAddItem(PortionData, 1);
	}
}

void UUIManager::LevelUpGun()
{
	APlayerCharacter* APC = GetOwningPlayer()->GetPawn<APlayerCharacter>();
	if(APC)
	{
		APC->GetWeaponSystem()->LevelUpGun();
	}
}

void UUIManager::AwakeGun()
{
	APlayerCharacter* APC = GetOwningPlayer()->GetPawn<APlayerCharacter>();
	if(APC)
	{
		APC->GetWeaponSystem()->AwakeGun();
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

void UUIManager::ToggleCombatUI(bool bOpen)
{
	CrossHairUI->Toggle(bOpen);
}

