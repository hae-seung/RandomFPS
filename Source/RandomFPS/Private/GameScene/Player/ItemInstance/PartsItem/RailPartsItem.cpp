// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/PartsItem/RailPartsItem.h"

#include "GameScene/Player/ItemData/PartsData/RailPartsData.h"
#include "Net/UnrealNetwork.h"

void URailPartsItem::Init(UItemData* Data)
{
	Super::Init(Data);
	RailPartsData = Cast<URailPartsData>(Data);
}

UItemInstance* URailPartsItem::CloneItem()
{
	URailPartsItem* NewItem = NewObject<URailPartsItem>(GetWorld());
	NewItem->Init(RailPartsData);
	return NewItem;
}

void URailPartsItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URailPartsItem, RailPartsData);
}
