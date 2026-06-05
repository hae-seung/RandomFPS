// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MenuController.h"
#include "MainMenu/MainMenuUI.h"
#include "Blueprint/UserWidget.h"

void AMenuController::BeginPlay()
{
	Super::BeginPlay();
	
	if(!IsLocalController())
		return;
	
	MainMenuUI = CreateWidget<UMainMenuUI>(this, BP_MainMenuUI);
	MainMenuUI->AddToViewport(0);

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}
