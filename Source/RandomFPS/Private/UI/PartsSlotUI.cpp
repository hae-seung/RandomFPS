// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PartsSlotUI.h"

#include "Components/Image.h"

void UPartsSlotUI::Init()
{
	bCanUse = true;
	bHasParts = false;
	
	HighlightImage->SetVisibility(ESlateVisibility::Collapsed);
	this->SetVisibility(ESlateVisibility::Collapsed);
}


FReply UPartsSlotUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if(bHasParts)
			RightMouseButtonClickedEvent.ExecuteIfBound(SlotPartsType);
		
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UPartsSlotUI::BindRightMouseButtonEvent(FRightMouseButtonClickedEvent ClickEvent)
{
	RightMouseButtonClickedEvent = ClickEvent;
}

void UPartsSlotUI::SetSlotUsable(bool value)
{
	bCanUse = value;
	if(bCanUse)
	{
		this->SetVisibility(ESlateVisibility::Visible);
		UpdateImage(nullptr);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPartsSlotUI::UpdateImage(UTexture2D* Image)
{
	if(Image != nullptr)
	{
		bHasParts = true;
		PartsImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		bHasParts = false;
		PartsImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	
	PartsImage->SetBrushFromTexture(Image);
}

void UPartsSlotUI::SetHighlight()
{
	HighlightImage->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPartsSlotUI::OffHighlight()
{
	HighlightImage->SetVisibility(ESlateVisibility::Collapsed);
}

