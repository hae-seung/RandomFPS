// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/EnumHeader/FInventoryList.h"

#include "GameScene/Player/ItemInstance/ItemInstance.h"


void FInventoryList::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	
}

void FInventoryList::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	
}


void FInventoryList::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	
}

void FInventoryList::UnBindItemEvent(int Index)
{
	if(Items[Index].ItemInstance != nullptr)
	{
		Items[Index].ItemInstance->OnItemStateChangedEvent.Unbind();
	}
}
