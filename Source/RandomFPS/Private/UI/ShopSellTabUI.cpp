// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopSellTabUI.h"

void UShopSellTabUI::Open()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UShopSellTabUI::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

