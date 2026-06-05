// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "ItemData.generated.h"


class UItemInstance;

UCLASS(Abstract)
class RANDOMFPS_API UItemData : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE FName GetItemId() const { return GetFName(); }
	FORCEINLINE FText GetItemName() const { return ItemName; }
	FORCEINLINE int32 GetMaxAmount() const { return MaxAmount; }
	FORCEINLINE UTexture2D* GetItemIcon() const { return ItemIcon; }
	FORCEINLINE FText GetItemDescription() const { return ItemDescription; }

	//abstract
	virtual UItemInstance* CreateItem(UObject* Outer) PURE_VIRTUAL(UItemData::CreateItem, return nullptr; );

	
private:
	FName ItemId;

	UPROPERTY(EditAnywhere)
	FText ItemName;

	UPROPERTY(EditAnywhere)
	int32 MaxAmount;

	UPROPERTY(EditAnywhere)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere)
	FText ItemDescription;
};
