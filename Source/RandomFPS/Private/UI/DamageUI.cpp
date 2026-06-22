// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageUI.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

void UDamageUI::SetDamage(float DamageAmount, bool bIsCritical)
{
	DamageText->SetText(FText::AsNumber(FMath::RoundToInt(DamageAmount)));

	if(bIsCritical)
	{
		
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
	else
	{
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f)));
	}
	
	PlayAnimation(PrintDamageAnim);
}
