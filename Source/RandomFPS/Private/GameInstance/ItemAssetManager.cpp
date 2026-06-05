// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/ItemAssetManager.h"
#include "GameScene/Player/ItemData/ItemData.h"

const FPrimaryAssetType UItemAssetManager::ItemDataType = TEXT("ItemData");


UItemAssetManager& UItemAssetManager::Get()
{
	return *CastChecked<UItemAssetManager>(GEngine->AssetManager);
}


UItemData* UItemAssetManager::GetItemData(FName ItemId) const
{
	const FPrimaryAssetId AssetId(ItemDataType, ItemId);
	
	UObject* Asset = GetPrimaryAssetObject(AssetId);

	return Cast<UItemData>(Asset);
}
