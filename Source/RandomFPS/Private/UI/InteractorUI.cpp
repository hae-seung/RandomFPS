// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractorUI.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "GameScene/Player/Components/PlayerInteractSystem.h"

void UInteractorUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ProgressMID = ProgressBarImage->GetDynamicMaterial();
	check(ProgressMID);

	Hide(true);
}

void UInteractorUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if(!bInteracting)
		return;
	
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateProgressTick();
}

void UInteractorUI::Init(UPlayerInteractSystem* InteractSystem)
{
	InteractSystem->OnInteractStart.AddUObject(this, &UInteractorUI::UpdateInteractUI);
	InteractSystem->OnInteractEnd.AddUObject(this, &UInteractorUI::Hide);
}


void UInteractorUI::Show()
{
	SetVisibility(ESlateVisibility::Visible);
	bInteracting = true;
}


void UInteractorUI::Hide(bool bSuccess)
{
	SetVisibility(ESlateVisibility::Collapsed);
	bInteracting = false;
}

void UInteractorUI::UpdateInteractUI(float EndTime, float InteractTotalTime, UTexture2D* Icon)
{
	InteractingImage->SetBrushFromTexture(Icon);
	ServerEndTime = EndTime;
	TotalTime = InteractTotalTime;
	
	Show();
}

void UInteractorUI::UpdateProgressTick()
{
	const float CurrentTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	const float RemainTime = ServerEndTime - CurrentTime;

	const float Progress = 1 - (RemainTime / TotalTime);
	
	ProgressMID->SetScalarParameterValue(
		TEXT("Progress"),
		FMath::Clamp(Progress, 0.f, 1.f));

	RemainText->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.1f"), RemainTime)
			));
}
