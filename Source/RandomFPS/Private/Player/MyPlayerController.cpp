// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/MyPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIManager.h"



void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(IsLocalPlayerController())
	{
		if(UEnhancedInputLocalPlayerSubsystem* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			SubSystem = Sub;
			
			if(IMC_Main)
				SubSystem->AddMappingContext(IMC_Main, 0);

			if(IMC_Mouse)
				SubSystem->AddMappingContext(IMC_Mouse, 1);
		}
	}
}

//서버전용
void AMyPlayerController::OnPossess(APawn* InPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPossess %s"), *GetName());
	Super::OnPossess(InPawn);
	CreateUIManager();
}

//클라
void AMyPlayerController::OnRep_Pawn()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Pawn %s"), *GetName());

	Super::OnRep_Pawn();
	CreateUIManager();
}

void AMyPlayerController::CreateUIManager()
{
	UE_LOG(LogTemp, Warning,
		TEXT("CreateUIManager Local=%d UIManager=%p Pawn=%s"),
		IsLocalController(),
		UIManager,
		*GetNameSafe(GetPawn()));
	if(!IsLocalController() || UIManager) return;
	
	UIManager = CreateWidget<UUIManager>(this, BP_UIManager);
	UIManager->AddToViewport();
	UIManager->Init(GetPawn());
}

void AMyPlayerController::SetInputModeUI()
{
	if(!IsLocalController()) return;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	bShowMouseCursor = true;
	
	SetInputMode(InputMode);

	SubSystem->RemoveMappingContext(IMC_Mouse);
}

void AMyPlayerController::SetInputModeGame()
{
	if(!IsLocalController()) return;
	
	FInputModeGameOnly InputMode;
	
	SetInputMode(InputMode);
	
	bShowMouseCursor = false;

	SubSystem->AddMappingContext(IMC_Mouse, 1);
}




