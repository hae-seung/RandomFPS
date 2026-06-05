// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/PartsItem/MuzzlePartsItem.h"

#include "GameScene/Player/ItemData/PartsData/MuzzlePartsData.h"
#include "Net/UnrealNetwork.h"

void UMuzzlePartsItem::Init(UItemData* Data)
{
	Super::Init(Data);
	MuzzlePartsData = Cast<UMuzzlePartsData>(Data);
}

UItemInstance* UMuzzlePartsItem::CloneItem()
{
	UMuzzlePartsItem* NewItem = NewObject<UMuzzlePartsItem>(GetWorld());
	NewItem->Init(MuzzlePartsData);
	return NewItem;
}

void UMuzzlePartsItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMuzzlePartsItem, MuzzlePartsData);
}
