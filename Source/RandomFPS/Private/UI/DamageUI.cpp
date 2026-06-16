// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageUI.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UDamageUI::Init(UWidgetComponent* WidgetComponent)
{
	PC = GetWorld()->GetFirstPlayerController();
	DamageWidgetComponent = WidgetComponent;
}

void UDamageUI::SetDamage(float DamageAmount, bool bIsCritical)
{
	DamageText->SetText(FText::AsNumber(FMath::RoundToInt(DamageAmount)));

	if(bIsCritical)
	{
		
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
	else
	{
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}

	LookCamera();

	PlayAnimation(PrintDamageAnim);
}

void UDamageUI::LookCamera()
{
	PC->GetPlayerViewPoint(CameraLocation, CameraRotator);

	LookAtRot = UKismetMathLibrary::FindLookAtRotation(
		DamageWidgetComponent->GetComponentLocation(),
		CameraLocation);

	DamageWidgetComponent->SetWorldRotation(LookAtRot);
}
