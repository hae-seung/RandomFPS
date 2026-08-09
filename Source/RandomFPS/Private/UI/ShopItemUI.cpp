// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopItemUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/ItemData/ItemData.h"


void UShopItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BuyBtn->OnReleased.AddDynamic(this, &UShopItemUI::OpenBuyQuantityTab);
}

void UShopItemUI::SetShopItemInfo(const FShopItemEntry& ShopItemEntry, EShopCategoryType InItemCategory)
{
	ItemEntry = &ShopItemEntry;
	ItemCategory = InItemCategory;

	ItemNameText->SetText(ShopItemEntry.ItemData->GetItemName());
	ItemImage->SetBrushFromTexture(
		ShopItemEntry.ItemData->GetItemIcon()
	);

	if (ShopItemEntry.Stock == -1)
	{
		StockText->SetText(FText::FromString(TEXT("수량 : ∞")));
	}
	else
	{
		StockText->SetText(
			FText::Format(
				FText::FromString(TEXT("수량 : {0}")),
				ShopItemEntry.Stock
			)
		);
	}
	
	BuyBtn->SetIsEnabled(ShopItemEntry.Stock != 0);
}

void UShopItemUI::OpenBuyQuantityTab()
{
	OnBuyBtnClicked.ExecuteIfBound(ItemEntry, ItemCategory);
}
