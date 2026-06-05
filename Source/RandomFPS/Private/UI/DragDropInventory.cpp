// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DragDropInventory.h"

#include "Components/Widget.h"

void UDragDropInventory::Init(int Index)
{
	SlotIndex = Index;
}

void UDragDropInventory::DragCancelled_Implementation(const FPointerEvent& PointerEvent)
{
	Super::DragCancelled_Implementation(PointerEvent);

	DefaultDragVisual->SetVisibility(ESlateVisibility::Collapsed);
}
