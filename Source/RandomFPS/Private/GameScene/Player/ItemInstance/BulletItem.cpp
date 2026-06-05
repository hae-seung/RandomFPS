// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/BulletItem.h"
#include "GameScene/Player/ItemData/BulletItemData.h"
#include "Net/UnrealNetwork.h"


void UBulletItem::Init(UItemData* Data)
{
	Super::Init(Data);
	BulletItemData = Cast<UBulletItemData>(Data);
}

UItemInstance* UBulletItem::CloneItem()
{
	UBulletItem* NewItem = NewObject<UBulletItem>(GetWorld());
	NewItem->Init(BulletItemData);
	return NewItem;
}

void UBulletItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBulletItem, BulletItemData);
}

