// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FInventoryItem.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "FInventoryList.generated.h"


class UInventory;

USTRUCT(Atomic)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FInventoryItem> Items;

	//inventory.h의 BeginPlay에서 할당
	UPROPERTY(NotReplicated)
	TObjectPtr<UInventory> Owner = nullptr;
	
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return Super::FastArrayDeltaSerialize<FInventoryItem, FInventoryList>(Items, DeltaParams, *this);
	}

	//FInventoryList내부 단위, 즉 배열 단위에서 변경되었을 때 콜백함수
	//MarkArrayDirty로 호출됨.
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	void UnBindItemEvent(int Index);
};



template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};