// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthUI.h"

#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHealthUI::Init(UWidgetComponent* HealthBarWidget)
{
	PC = GetWorld()->GetFirstPlayerController();
	HealthBarWidgetComp = HealthBarWidget;
	HideHealth();
}


void UHealthUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!bIsOpen)
		return;
	
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	LookAtRot =
		UKismetMathLibrary::FindLookAtRotation(
			HealthBarWidgetComp->GetComponentLocation(),
			CameraLocation);

	HealthBarWidgetComp->SetWorldRotation(LookAtRot);
}

void UHealthUI::UpdateHealth(float Hp, float MaxHp)
{
	bIsOpen = true;
	HealthBar->SetPercent(Hp / MaxHp);
	
	SetVisibility(ESlateVisibility::HitTestInvisible);
}


void UHealthUI::HideHealth()
{
	bIsOpen = false;
	SetVisibility(ESlateVisibility::Collapsed);
}
