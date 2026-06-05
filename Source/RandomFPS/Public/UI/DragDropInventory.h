// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragDropInventory.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UDragDropInventory : public UDragDropOperation
{
	GENERATED_BODY()

public:
	int32 SlotIndex;
	
public:
	void Init(int Index);

private:
	virtual void DragCancelled_Implementation(const FPointerEvent& PointerEvent) override;
};
