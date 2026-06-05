// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DragSlot.h"

#include "Components/Image.h"

void UDragSlot::SetImage(UTexture2D* IconImage)
{
	ItemImage->SetBrushFromTexture(IconImage);
	SetVisibility(ESlateVisibility::HitTestInvisible);
}
