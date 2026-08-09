// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameScene/InteractableObject/InteractableUIObject/Shop.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerWalletSystem.h"
#include "UI/BuyQuantityTab.h"
#include "UI/ShopBuyTabUI.h"
#include "UI/ShopSellTabUI.h"

void UShopUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	//WidgetInteraction과 상호작용 하면 Click이 안되는 버그가 있음.
	BuyBtn->OnReleased.AddDynamic(this, &UShopUI::OpenBuyTab);
	SellBtn->OnReleased.AddDynamic(this, &UShopUI::OpenSellTab);
	CloseBtn->OnReleased.AddDynamic(this ,&UShopUI::CloseShopUI);
}


void UShopUI::FirstOpenInit(AShop* ShopObject, APlayerCharacter* InAPC)
{
	APC = InAPC;
	ShopActor = ShopObject;
	
	//상점데이터 객체를 토대로 버튼 생성 및 초기화
	BuyTabUI->FirstOpenInit(ShopObject->GetClientShopState(), this);
	BuyQuantityTab->InitFirstOpen(APC->GetWalletSystem());
	BuyQuantityTab->OnBuyConfirm.BindUObject(ShopObject, &AShop::RequestBuyItem);
	
	BuyBtn->SetIsEnabled(false);
}

void UShopUI::Open()
{
	Super::Open();

	BuyQuantityTab->Close();
	UserMoneyText->SetText(FText::Format(
		FText::FromString(TEXT("{0}원")),
		APC->GetWalletSystem()->GetMoney()));
}

void UShopUI::CloseShopUI()
{
	ShopActor->CloseShopUI();
}


void UShopUI::OpenBuyTab()
{
	SellTabUI->Close();
	BuyTabUI->Open();
	
	SellBtn->SetIsEnabled(true);
	BuyBtn->SetIsEnabled(false);
}

void UShopUI::OpenSellTab()
{
	BuyTabUI->Close();
	SellTabUI->Open();

	SellBtn->SetIsEnabled(false);
	BuyBtn->SetIsEnabled(true);
}

void UShopUI::OpenBuyQuantityTab(const FShopItemEntry* ShopItemEntry, EShopCategoryType InItemCategory)
{
	BuyQuantityTab->SetQuantityTab(ShopItemEntry, InItemCategory);
	BuyQuantityTab->Open();
}

void UShopUI::FailToBuy()
{
	//일단 서버로부터 응답을 받긴했음
	BuyQuantityTab->SetSyncState(true);
}

void UShopUI::BuySuccess(const FShopBuyContextFeedback& BuyContextFeedback)
{
	BuyQuantityTab->SetSyncState(true);
	
	BuyTabUI->UpdateShopItemByFeedback(BuyContextFeedback);
	
	UserMoneyText->SetText(FText::Format(
		FText::FromString(TEXT("{0}원")),
		BuyContextFeedback.RemainPlayerMoney));
}
