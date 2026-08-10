// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BuyQuantityTab.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/Components/PlayerWalletSystem.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "UI/AmountDeltaButton.h"

void UBuyQuantityTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BuyBtn->OnReleased.AddDynamic(this ,&UBuyQuantityTab::ConfirmBuy);
	CancelBtn->OnReleased.AddDynamic(this, &UBuyQuantityTab::Close);

	bIsSyncComplete = true;
}

void UBuyQuantityTab::InitFirstOpen(UPlayerWalletSystem* InWalletSystem)
{
	PlayerWallet = InWalletSystem;

	AmountMinus1Btn->InitFirstOpen();
	AmountMinus5Btn->InitFirstOpen();
	AmountMinus10Btn->InitFirstOpen();
	AmountPlus1Btn->InitFirstOpen();
	AmountPlus5Btn->InitFirstOpen();
	AmountPlus10Btn->InitFirstOpen();

	AmountMinus1Btn->OnCustomButtonReleased.AddUObject(this, &UBuyQuantityTab::ChangeSelectAmount);
	AmountMinus5Btn->OnCustomButtonReleased.AddUObject(this, &UBuyQuantityTab::ChangeSelectAmount);
	AmountMinus10Btn->OnCustomButtonReleased.AddUObject(this, &UBuyQuantityTab::ChangeSelectAmount);
	AmountPlus1Btn->OnCustomButtonReleased.AddUObject(this, &UBuyQuantityTab::ChangeSelectAmount);
	AmountPlus5Btn->OnCustomButtonReleased.AddUObject(this, &UBuyQuantityTab::ChangeSelectAmount);
	AmountPlus10Btn->OnCustomButtonReleased.AddUObject(this, &UBuyQuantityTab::ChangeSelectAmount);
}

void UBuyQuantityTab::Open()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UBuyQuantityTab::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}


void UBuyQuantityTab::SetQuantityTab(const FShopItemEntry* EntryItem, EShopCategoryType InItemCategory)
{
	ItemImage->SetBrushFromTexture(EntryItem->ItemData->GetItemIcon());
	ItemNameText->SetText(EntryItem->ItemData->GetItemName());

	if(EntryItem->Stock == -1)
	{
		StockText->SetText(FText::FromString(TEXT("수량 : ∞")));
	}
	else
	{
		StockText->SetText(FText::Format(
		FText::FromString(TEXT("재고 : {0}")), EntryItem->Stock));
	}

	PriceText->SetText(FText::Format(
		FText::FromString(TEXT("개당가격 : {0}원")),
		EntryItem->Price));
	
	FinalPriceText->SetText(FText::FromString(TEXT("최종가격 : 0원")));

	ItemAmountSelectText->SetText(FText::AsNumber(0));

	
	CurItem = EntryItem;
	CurSelectAmount = 0;
	TotalPrice = 0;
	ItemCategory = InItemCategory;
	BuyBtn->SetIsEnabled(false);
}

void UBuyQuantityTab::SetSyncState(bool bState)
{
	bIsSyncComplete = bState;
}

void UBuyQuantityTab::ChangeSelectAmount(int Delta)
{
	const int Money = PlayerWallet->GetMoney();
	int CanBuyMaxAmount = Money / CurItem->Price;
	if(CurItem->Stock > 0)
	{
		CanBuyMaxAmount = FMath::Min(CanBuyMaxAmount, CurItem->Stock);
	}
	
	CurSelectAmount = FMath::Clamp(CurSelectAmount + Delta, 0, CanBuyMaxAmount);
	TotalPrice = CurSelectAmount * CurItem->Price;
	
	ItemAmountSelectText->SetText(FText::AsNumber(CurSelectAmount));
	FinalPriceText->SetText(FText::Format(
		FText::FromString(TEXT("최종가격 : {0}원")),
		TotalPrice));

	BuyBtn->SetIsEnabled(CurSelectAmount > 0);
}

void UBuyQuantityTab::ConfirmBuy()
{
	/*
	 * 서버로부터 구매 상황이 클라에 도착하여 싱크가 맞춰져야만 실제 물건 구매 가능
	 * 싱크가 안맞으면 작동x
	 */
	if(!bIsSyncComplete)
		return;


	//ShopUI에 Context로 요청
	const FShopBuyContext BuyContext{
		ItemCategory,
		CurItem->ItemData->GetItemId(),
		CurSelectAmount};

	bIsSyncComplete = false;
	
	OnBuyConfirm.ExecuteIfBound(BuyContext);

	Close();
}
