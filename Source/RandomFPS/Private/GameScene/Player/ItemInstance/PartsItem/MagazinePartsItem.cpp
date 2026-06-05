// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/PartsItem/MagazinePartsItem.h"
#include "GameScene/Player/ItemData/PartsData/MagazinePartsData.h"
#include "Net/UnrealNetwork.h"

void UMagazinePartsItem::Init(UItemData* Data)
{
	Super::Init(Data);
	MagazinePartsData = Cast<UMagazinePartsData>(Data);
}

UItemInstance* UMagazinePartsItem::CloneItem()
{
	UMagazinePartsItem* NewItem = NewObject<UMagazinePartsItem>(GetWorld());
	NewItem->Init(MagazinePartsData);
	return NewItem;
}

void UMagazinePartsItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMagazinePartsItem, MagazinePartsData);
}

int UMagazinePartsItem::GetAdditiveMaxMagAmmo()
{
	return MagazinePartsData->MaxMagAmmo;
}

