// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "FInventoryItem.generated.h"


class UItemInstance;


DECLARE_DELEGATE_OneParam(FOnItemInstanceStateChanged, int32)

USTRUCT()
struct FInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FOnItemInstanceStateChanged OnItemInstanceStateChangedEvent;
	
	UPROPERTY()
	int32 SlotIndex;
	
	UPROPERTY()
	UItemInstance* ItemInstance;

	//FInventoryItem 내부에서 변화가 일어남을 감지해서 자동으로 호출
	//MarkItemDirty()를 하면 Delta를 계산해서 3개 중 한개를 호출
	void PostReplicatedAdd(const struct FInventoryList& Array);
	void PreReplicatedRemove(const struct FInventoryList& Array);
	void PostReplicatedChange(const struct FInventoryList& Array);
};
