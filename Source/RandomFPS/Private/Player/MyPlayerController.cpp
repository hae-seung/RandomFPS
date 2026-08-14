// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/MyPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/Player/MyPlayerState.h"
#include "GameScene/Player/ControllerHubs/ReinforceShopHub.h"
#include "GameScene/Player/ControllerHubs/ShopHub.h"
#include "UI/UIManager.h"

AMyPlayerController::AMyPlayerController()
{
	ShopHub = CreateDefaultSubobject<UShopHub>(TEXT("ShopHub"));
	ReinforceShopHub = CreateDefaultSubobject<UReinforceShopHub>(TEXT("ReinforceShopHub"));
}


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

			if(IMC_Mouse_Weapon)
				SubSystem->AddMappingContext(IMC_Mouse_Weapon, 1);
		}
	}
}

//서버전용 한번에 모두 존재함
void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CreateUIManager();
}

//클라
void AMyPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	CreateUIManager();
}

//클라
void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	CreateUIManager();
}


void AMyPlayerController::CreateUIManager()
{
	//Pawn이랑 State중 뭐가 먼저 도착할지 알 수 없음
	if(!IsLocalController() || UIManager ||
		!IsValid(GetPawn()) ||
		!IsValid(PlayerState))
		return;
	
	UIManager = CreateWidget<UUIManager>(this, BP_UIManager);
	UIManager->AddToViewport();
	UIManager->Init(GetPawn());
}

void AMyPlayerController::SetInputModeUI()
{
	if(!IsLocalController()) return;

	ToggleCombatUI(false);
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	//마우스 꾹 누르면 마우스 포인터 사라지는 상황 막아줌
	InputMode.SetHideCursorDuringCapture(false);
	bShowMouseCursor = true;

	//마우스를 꾹 누른채로 회전 되는 상황 막아줌
	SetIgnoreLookInput(true);
	
	SetInputMode(InputMode);

	SubSystem->RemoveMappingContext(IMC_Mouse_Weapon);
}

void AMyPlayerController::SetInputModeGame()
{
	if(!IsLocalController()) return;

	ToggleCombatUI(true);
	
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	
	SetIgnoreLookInput(false);
	SubSystem->AddMappingContext(IMC_Mouse_Weapon, 1);
}

void AMyPlayerController::ToggleCombatUI(bool bState)
{
	UIManager->ToggleCombatUI(bState);
}

void AMyPlayerController::ChangeWidgetInteractionMode(bool bState)
{
	if(bState)
	{
		SubSystem->AddMappingContext(IMC_Mouse_RayCast, 1);
	}
	else
	{
		SubSystem->RemoveMappingContext(IMC_Mouse_RayCast);
	}
}


