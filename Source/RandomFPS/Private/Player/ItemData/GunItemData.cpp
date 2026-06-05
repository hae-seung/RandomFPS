// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemData/GunItemData.h"

#include "GameScene/Player/ItemInstance/GunItem.h"

UItemInstance* UGunItemData::CreateItem(UObject* Outer)
{
	UGunItem* NewItem = NewObject<UGunItem>(Outer);
	NewItem->Init(this);
	return NewItem;
}
