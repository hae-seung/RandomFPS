// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "ShopStruct.generated.h"


class UItemData;

USTRUCT(BlueprintType)
struct FShopItemEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemData> ItemData;

	UPROPERTY(EditAnywhere)
	int32 Stock;

	UPROPERTY(EditAnywhere)
	int32 Price;
};


USTRUCT(BlueprintType)
struct FShopCategory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EShopCategoryType CategoryType;

	UPROPERTY(EditAnywhere)
	TArray<FShopItemEntry> Items;
};


USTRUCT(BlueprintType)
struct FShopState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FShopCategory> Categories;
};

USTRUCT()
struct FShopBuyContext
{
	GENERATED_BODY()

	UPROPERTY()
	EShopCategoryType ItemType;
	UPROPERTY()
	FName ItemID;
	UPROPERTY()
	int BuyAmount;
};

USTRUCT()
struct FShopBuyContextFeedback
{
	GENERATED_BODY()

	UPROPERTY()
	EShopCategoryType ItemType;
	UPROPERTY()
	FName ItemID;
	UPROPERTY()
	int BuySuccessAmount;
	UPROPERTY()
	int RemainPlayerMoney;
};

USTRUCT()
struct FShopSellEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UItemData* ItemData;
	UPROPERTY(EditAnywhere)
	int PerPrice;
};

USTRUCT()
struct FShopSellContext
{
	GENERATED_BODY()

	UPROPERTY()
	FName ItemID;
	UPROPERTY()
	int InventoryIndex;
	UPROPERTY()
	int SellAmount;
};

USTRUCT()
struct FShopSellContextFeedback
{
	GENERATED_BODY()

	UPROPERTY()
	FName ItemID;
	UPROPERTY()
	int RemainPlayerMoney;
	UPROPERTY()
	int ItemRemainStock;
};
