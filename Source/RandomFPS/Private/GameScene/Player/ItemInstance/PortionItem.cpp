// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/PortionItem.h"

#include "GameScene/Player/ItemData/PortionItemData.h"
#include "Net/UnrealNetwork.h"

void UPortionItem::Init(UItemData* Data)
{
	Super::Init(Data);
	PortionItemData = Cast<UPortionItemData>(Data);
}

UItemInstance* UPortionItem::CloneItem()
{
	UPortionItem* NewItem = NewObject<UPortionItem>(GetWorld());
	NewItem->Init(PortionItemData);
	return NewItem;
}

void UPortionItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPortionItem, PortionItemData);
}
