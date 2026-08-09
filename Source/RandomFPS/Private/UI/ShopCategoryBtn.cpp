// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopCategoryBtn.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UShopCategoryBtn::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CategoryBtn->OnReleased.AddDynamic(this, &UShopCategoryBtn::UShopCategoryBtn::OpenCategoryList);
}

void UShopCategoryBtn::Init(EShopCategoryType CategoryType, FText ButtonName)
{
	ShopCategoryType = CategoryType;
	CategoryNameText->SetText(ButtonName);
}

void UShopCategoryBtn::SetSelectState(bool bState)
{
	CategoryBtn->SetIsEnabled(!bState);
}

void UShopCategoryBtn::OpenCategoryList()
{
	CategoryBtn->SetIsEnabled(false);
	OnCategoryBtnClicked.ExecuteIfBound(ShopCategoryType, this);
}
