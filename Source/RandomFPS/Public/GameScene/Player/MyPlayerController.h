// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "GameScene/EnumHeader/EnumHeader.h"

#include "MyPlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class APlayerCharacter;
class UUIManager;
class UInputMappingContext;


UCLASS()
class RANDOMFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_Main;
	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_Mouse;
	FORCEINLINE UUIManager* GetUIManager() const { return UIManager; }


public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;
	
	//UI를 켤 때와 끌 때
	void SetInputModeUI();
	void SetInputModeGame();

	
private:
	UEnhancedInputLocalPlayerSubsystem* SubSystem;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	TSubclassOf<UUIManager> BP_UIManager;
	UUIManager* UIManager;



	
	
private:
	void CreateUIManager();
	
};
