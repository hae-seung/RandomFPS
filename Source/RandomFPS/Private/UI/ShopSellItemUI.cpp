// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopSellItemUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"


void UShopSellItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SellItemBtn->OnReleased.AddDynamic(this, &UShopSellItemUI::SellItemClicked);
}

void UShopSellItemUI::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UShopSellItemUI::SetCanClickState(bool bState)
{
	SellItemBtn->SetIsEnabled(bState);
}

void UShopSellItemUI::SetRemainStock(int RemainAmount)
{
	StockText->SetText(FText::Format(
		FText::FromString(TEXT("x{0}")),
		RemainAmount));
}

void UShopSellItemUI::SellItemClicked()
{
	OnClickedSellItemUI.ExecuteIfBound(ItemInstance, ItemInventoryIndex, SlotIndex, ItemPrice);
}

void UShopSellItemUI::SetUIIndex(int Index)
{
	SlotIndex = Index;
}

void UShopSellItemUI::SetItemInfoAndOpen(const FInventoryItem& Item, int PerPrice)
{
	ItemImage->SetBrushFromTexture(Item.ItemInstance->ItemData->GetItemIcon());
	
	StockText->SetText(FText::Format(
		FText::FromString(TEXT("x{0}")),
		Item.ItemInstance->ItemAmount));
	
	PerPriceText->SetText(FText::Format(
		FText::FromString(TEXT("{0}원")),
		PerPrice));

	
	ItemInstance = Item.ItemInstance;
	ItemInventoryIndex = Item.SlotIndex;
	ItemPrice = PerPrice;

	SetVisibility(ESlateVisibility::Visible);
}
