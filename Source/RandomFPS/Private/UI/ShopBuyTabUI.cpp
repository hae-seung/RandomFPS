// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopBuyTabUI.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/WrapBox.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "Public/UI/ShopCategoryBtn.h"
#include "UI/ShopItemUI.h"

void UShopBuyTabUI::Open()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UShopBuyTabUI::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UShopBuyTabUI::FirstOpenInit(FShopState& ShopInstance, UShopUI* InShopUI)
{
	CachedShopState = &ShopInstance;
	ShopUI = InShopUI;
	
	//버튼 생성
	const TArray<FShopCategory>& Categories = CachedShopState->Categories;
	for(int i = 0; i < Categories.Num(); i++)
	{
		EShopCategoryType ShopCategory = Categories[i].CategoryType;
		UShopCategoryBtn* NewBtn = CreateWidget<UShopCategoryBtn>(this, WB_CategoryBtn);
		NewBtn->Init(ShopCategory, ShopCategoryNameMap.FindRef(ShopCategory));
		NewBtn->OnCategoryBtnClicked.BindUObject(this, &UShopBuyTabUI::UpdateItemList);
		UHorizontalBoxSlot* NewSlot = CategoryButtonBox->AddChildToHorizontalBox(NewBtn);
		if (i < Categories.Num() - 1)
		{
			NewSlot->SetPadding(CategoryButtonPadding);
		}

		if(i == 0)
		{
			UpdateItemList(ShopCategory, NewBtn);
		}
	}
}

void UShopBuyTabUI::UpdateItemList(EShopCategoryType CategoryType, UShopCategoryBtn* NewSelectBtn)
{
	if(CurSelectedBtn)
	{
		CurSelectedBtn->SetSelectState(false);
	}
	CurSelectedBtn = NewSelectBtn;
	CurSelectedBtn->SetSelectState(true);
	CurSelectedCategory = CategoryType;
	
	const TArray<FShopItemEntry>* Items = GetShopItems(CategoryType);

	if (!Items)
	{
		return;
	}

	while (ShopItemUIs.Num() < Items->Num())
	{
		AddNewShopItemUI();
	}

	int i = 0;

	for (; i < Items->Num(); ++i)
	{
		ShopItemUIs[i]->SetShopItemInfo((*Items)[i], CategoryType);
		ShopItemUIs[i]->SetVisibility(ESlateVisibility::Visible);
	}

	for (; i < ShopItemUIs.Num(); ++i)
	{
		ShopItemUIs[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

const TArray<FShopItemEntry>* UShopBuyTabUI::GetShopItems(EShopCategoryType CategoryType)
{
	const TArray<FShopCategory>& Categories = CachedShopState->Categories;
	for(int i = 0; i < Categories.Num(); i++)
	{
		if(Categories[i].CategoryType == CategoryType)
		{
			return &Categories[i].Items;
		}
	}

	return nullptr;
}

void UShopBuyTabUI::AddNewShopItemUI()
{
	UShopItemUI* NewShopUI = CreateWidget<UShopItemUI>(this, WB_ShopItemUI);
	NewShopUI->OnBuyBtnClicked.BindUObject(ShopUI, &UShopUI::OpenBuyQuantityTab);
	ShopItemUIs.Add(NewShopUI);
	ItemList->AddChild(NewShopUI);

	NewShopUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UShopBuyTabUI::UpdateShopItemByFeedback(const FShopBuyContextFeedback& BuyContextFeedback)
{
	FShopCategory* ShopCategory = CachedShopState->Categories.FindByPredicate(
		[&BuyContextFeedback](const FShopCategory& Category)
		{
			return Category.CategoryType == BuyContextFeedback.ItemType;
		});
	

	for(int i = 0; i < ShopCategory->Items.Num(); i++)
	{
		FShopItemEntry& ShopItem = ShopCategory->Items[i];
		if(ShopItem.ItemData->GetItemId() == BuyContextFeedback.ItemID)
		{
			if(ShopItem.Stock != -1)
				ShopItem.Stock -= BuyContextFeedback.BuySuccessAmount;

			//현재 피드백 받은 아이템 화면을 보고 있었다면
			//화면의 ShopItemUI도 수정
			if(CurSelectedCategory == BuyContextFeedback.ItemType)
			{
				ShopItemUIs[i]->SetShopItemInfo(ShopItem, BuyContextFeedback.ItemType);
			}

			return;
		}
	}
}
