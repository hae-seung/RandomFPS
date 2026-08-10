// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemData/ShopData.h"

const FShopState& UShopData::GetShopData() const
{
	return ShopData;
}

const TArray<FShopSellEntry>& UShopData::GetShopSellData() const
{
	return ShopSellData;
}
