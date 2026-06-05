// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemData/PartsData/MagazinePartsData.h"

#include "GameScene/Player/ItemInstance/PartsItem/MagazinePartsItem.h"

UItemInstance* UMagazinePartsData::CreateItem(UObject* Outer)
{
	UMagazinePartsItem* NewItem = NewObject<UMagazinePartsItem>(Outer);
	NewItem->Init(this);
	return NewItem;
}
