// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WorldInteractionUI.h"

void UWorldInteractionUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Close();
}

void UWorldInteractionUI::Open()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UWorldInteractionUI::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

