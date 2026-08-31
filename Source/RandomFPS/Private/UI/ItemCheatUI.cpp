// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemCheatUI.h"

#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerWeapon.h"
#include "UI/CheatButton.h"


void UItemCheatUI::Init(APlayerCharacter* InAPC, AMyPlayerController* MPC)
{
	APC = InAPC;
	MyController = MPC;
	SetVisibility(ESlateVisibility::Collapsed);
}

void UItemCheatUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RedDotBtn->Init();
	ScopeBtn->Init();
	HoloBtn->Init();
	MagazineBtn->Init();
	SuppressorBtn->Init();
	CompensatorBtn->Init();
	BulletBtn->Init();
	PortionBtn->Init();
	
	
	RedDotBtn->OnCheatBtnClicked.BindUObject(this, &UItemCheatUI::GiveItem);
	ScopeBtn->OnCheatBtnClicked.BindUObject(this, &UItemCheatUI::GiveItem);
	HoloBtn->OnCheatBtnClicked.BindUObject(this, &UItemCheatUI::GiveItem);
	MagazineBtn->OnCheatBtnClicked.BindUObject(this, &UItemCheatUI::GiveItem);
	SuppressorBtn->OnCheatBtnClicked.BindUObject(this, &UItemCheatUI::GiveItem);
	CompensatorBtn->OnCheatBtnClicked.BindUObject(this, &UItemCheatUI::GiveItem);
	BulletBtn->OnCheatBtnClicked.BindUObject(this, &UItemCheatUI::GiveItem);
	PortionBtn->OnCheatBtnClicked.BindUObject(this, &UItemCheatUI::GiveItem);

	LevelUpBtn->OnClicked.AddDynamic(this, &UItemCheatUI::LevelUP);
	AwakeBtn->OnClicked.AddDynamic(this, &UItemCheatUI::Awake);

	bIsOpen= false;
}


void UItemCheatUI::GiveItem(UItemData* ItemData)
{
	if(!APC)
		return;
	
	APC->RequestAddItem(ItemData);
}

void UItemCheatUI::LevelUP()
{
	if(!APC)
		return;
	
	APC->GetWeaponSystem()->LevelUpGun();
}

void UItemCheatUI::Awake()
{
	if(!APC)
		return;
	
	APC->GetWeaponSystem()->LevelUpGun();
}

void UItemCheatUI::Toggle()
{
	if(bIsOpen)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		bIsOpen = false;
		MyController->SetInputModeGame();
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
		bIsOpen = true;
		MyController->SetInputModeUI();
	}
}

