// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemData/PortionItemData.h"

#include "GameScene/Player/ItemInstance/PortionItem.h"

UItemInstance* UPortionItemData::CreateItem(UObject* Outer)
{
	UPortionItem* NewItem = NewObject<UPortionItem>(Outer);
	NewItem->Init(this);
	return NewItem;
}
