// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TooltipUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"

void UTooltipUI::NativeOnInitialized()
{
	SetVisibility(ESlateVisibility::Collapsed);
}


void UTooltipUI::OpenToolTip(UItemInstance* Item)
{
	SetVisibility(ESlateVisibility::Visible);

	ItemImage->SetBrushFromTexture(Item->ItemData->GetItemIcon());
	ItemNameTxt->SetText(Item->ItemData->GetItemName());
	ItemDescriptionTxt->SetText(Item->ItemData->GetItemDescription());

	//사용가능한 아이템인지 인터페이스 확인
	
	
}

void UTooltipUI::CloseToolTip()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
