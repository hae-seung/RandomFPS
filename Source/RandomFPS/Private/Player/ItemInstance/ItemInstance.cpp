// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/ItemInstance.h"

#include "GameScene/Player/ItemData/ItemData.h"
#include "Net/UnrealNetwork.h"



void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemInstance, ItemAmount);
	DOREPLIFETIME(UItemInstance, ItemData);
}

bool UItemInstance::IsSupportedForNetworking() const
{
	return true;
}

FName UItemInstance::GetItemId()
{
	return ItemData->GetItemId();
}

void UItemInstance::Init(UItemData* Data)
{
	ItemData = Data;
	SetAmount(1);
}

bool UItemInstance::IsMax()
{
	return ItemAmount == ItemData->GetMaxAmount();
}

int32 UItemInstance::GetMaxAmount()
{
	if(!IsValid(ItemData))
	{
		return 0;
	}
	return ItemData->GetMaxAmount();
}

void UItemInstance::SetAmount(int Amount)
{
	ItemAmount = FMath::Clamp(Amount, 0, GetMaxAmount());
}

void UItemInstance::SetSlotIndex(int Index)
{
	SlotIndex = Index;
}

int32 UItemInstance::AddAmountAndGetExcess(int32 Amount)
{
	int NextAmount = Amount + ItemAmount;
	SetAmount(NextAmount);

	return NextAmount > GetMaxAmount() ? NextAmount - GetMaxAmount() : 0;
}


void UItemInstance::OnRep_ItemAmount()
{
	OnItemStateChangedEvent.ExecuteIfBound(SlotIndex);
}
