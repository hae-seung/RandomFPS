// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/PartsItem.h"

#include "GameScene/Player/ItemData/PartsItemData.h"
#include "Net/UnrealNetwork.h"

EPartsType UPartsItem::GetPartsType() const
{
	return PartsItemData->GetPartsType();
}

void UPartsItem::Init(UItemData* Data)
{
	Super::Init(Data);
	PartsItemData = Cast<UPartsItemData>(Data);
}

void UPartsItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPartsItem, PartsItemData);
}
