// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemData/PartsData/RailPartsData.h"
#include "GameScene/Player/ItemInstance/PartsItem/RailPartsItem.h"

UItemInstance* URailPartsData::CreateItem(UObject* Outer)
{
	URailPartsItem* NewItem = NewObject<URailPartsItem>(Outer);
	NewItem->Init(this);
	return NewItem;
}