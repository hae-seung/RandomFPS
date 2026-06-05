// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/EnumHeader/FInventoryItem.h"

#include "GameScene/EnumHeader/FInventoryList.h"
#include "GameScene/Player/Components/Inventory.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"


void FInventoryItem::PostReplicatedAdd(const FInventoryList& Array)
{
	//배열에 새로운 FInventoryItem가 Add된 경우
	//로컬이 아닌경우 return;
	if(!Array.Owner) return;
	
	Array.Owner->OnRep_AddItem(SlotIndex);
}

void FInventoryItem::PostReplicatedChange(const FInventoryList& Array)
{
	//배열에서 FInventoryItem 내부 값이 변경된 경우
	//즉 ItemInstance가 바뀐 경우
	if(!Array.Owner) return;

	if(ItemInstance != nullptr)
	{
		ItemInstance->SetSlotIndex(SlotIndex);
		ItemInstance->OnItemStateChangedEvent.BindUObject(Array.Owner, &UInventory::UpdateSlot);
	}
	
	//FItemInstance 포인터가 변경되었을때
	Array.Owner->UpdateSlot(SlotIndex);
}

void FInventoryItem::PreReplicatedRemove(const FInventoryList& Array)
{
	//배열에서 FInventoryItem가 Remove된 경우
	//쓸일 없음
}
