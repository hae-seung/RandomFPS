// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemData/BulletItemData.h"

#include "GameScene/Player/ItemInstance/BulletItem.h"

UItemInstance* UBulletItemData::CreateItem(UObject* Outer)
{
	UBulletItem* NewItem = NewObject<UBulletItem>(Outer);
	NewItem->Init(this);
	return NewItem;
}
