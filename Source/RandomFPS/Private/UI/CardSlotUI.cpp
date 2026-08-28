// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CardSlotUI.h"

#include "Components/Image.h"
#include "GameScene/Cards/CardData.h"


void UCardSlotUI::SetCard(UCardData* NewCardData)
{
	CardData = NewCardData;
	CardIcon->SetColorAndOpacity(FLinearColor(1.f,1.f,1.f,1.f));
	CardIcon->SetBrushFromTexture(CardData->GetCardIcon());
}

void UCardSlotUI::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if(CardData == nullptr)
		return;
	
	const FVector2D TopRightLocal(InGeometry.GetLocalSize().X, 0.f);
	const FVector2D ScreenPos = InGeometry.LocalToAbsolute(TopRightLocal);
	
	OnMouseEnterSlot.ExecuteIfBound(CardData->GetCardDescription(), ScreenPos);
}

void UCardSlotUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnMouseLeaveSlot.ExecuteIfBound();
}
