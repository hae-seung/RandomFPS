// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIManager.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameFramework/PlayerState.h"
#include "GameScene/Player/MyPlayerController.h"
#include "UI/CardMenuUI.h"
#include "UI/CombatUI.h"
#include "UI/CrossHairUI.h"
#include "UI/GunMenu.h"
#include "UI/InteractorUI.h"
#include "UI/InventoryUI.h"
#include "UI/ItemCheatUI.h"
#include "UI/KillLogUI.h"
#include "UI/RoundUI.h"
#include "UI/ScoreUI.h"
#include "UI/WalletUI.h"


void UUIManager::Init(APawn* Pawn)
{
	APlayerCharacter* APC = Cast<APlayerCharacter>(Pawn);
	LocalController = Cast<AMyPlayerController>(GetOwningPlayer());
	
	InventoryUI->Init(APC->GetInventory(), APC->GetStatComponent(),
		APC->GetWeaponSystem(), APC->GetCardSystem());
	GunMenuUI->Init(InventoryUI);
	CombatUI->Init(APC->GetCombatComponent(), APC->GetStatComponent(), APC->GetWeaponSystem());
	CrossHairUI->Init(APC->GetCombatComponent(), APC->GetWeaponSystem());
	ScoreUI->FindLocalEntry(GetOwningPlayerState());
	KillLogUI->Init(APC->KillAlarmEvent, APC->AssistAlarmEvent);
	InteractorUI->Init(APC->GetInteractSystem());
	WalletUI->Init(APC->GetWalletSystem());
	CardMenuUI->Init(APC->GetCardSystem());
	CheatUI->Init(APC, LocalController);
	
	RoundUI->Init();
}

void UUIManager::ToggleInventory()
{
	InventoryUI->Toggle();
}

void UUIManager::ToggleCheatUI()
{
	CheatUI->Toggle();
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

	if(bOpen)
	{
		CombatUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CombatUI->SetVisibility(ESlateVisibility::Collapsed);
	}
}

