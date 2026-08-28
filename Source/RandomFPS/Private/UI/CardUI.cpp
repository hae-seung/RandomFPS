// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CardUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameScene/Cards/CardData.h"


void UCardUI::NativeOnInitialized()
{
	CardBtn->OnClicked.AddDynamic(this, &UCardUI::SelectCardBtnClicked);
}

void UCardUI::Show()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UCardUI::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UCardUI::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	SizeBox->SetWidthOverride(BigSize.X);
	SizeBox->SetHeightOverride(BigSize.Y);
}

void UCardUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	SizeBox->SetWidthOverride(NormalSize.X);
	SizeBox->SetHeightOverride(NormalSize.Y);
}

void UCardUI::SelectCardBtnClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("카드 선택됨"));
	OnCardSelected.ExecuteIfBound(CardID);
}


void UCardUI::SetCardInfo(UCardData* CardData)
{
	CardNameText->SetText(CardData->GetCardName());
	CardIcon->SetBrushFromTexture(CardData->GetCardIcon());
	CardDescriptionText->SetText(CardData->GetCardDescription());
	CardID = CardData->GetCardId();
}

