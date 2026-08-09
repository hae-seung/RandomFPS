// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionUI.h"

#include "Components/TextBlock.h"


void UInteractionUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Hide();
}

void UInteractionUI::Show(FText Msg)
{
	SetVisibility(ESlateVisibility::Visible);
	InteractionMsgText->SetText(Msg);
}

void UInteractionUI::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}