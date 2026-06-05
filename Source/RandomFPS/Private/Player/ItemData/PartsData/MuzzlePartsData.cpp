// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemData/PartsData/MuzzlePartsData.h"
#include "GameScene/Player/ItemInstance/PartsItem/MuzzlePartsItem.h"


UItemInstance* UMuzzlePartsData::CreateItem(UObject* Outer)
{
	UMuzzlePartsItem* NewItem = NewObject<UMuzzlePartsItem>(Outer);
	NewItem->Init(this);
	return NewItem;
}
