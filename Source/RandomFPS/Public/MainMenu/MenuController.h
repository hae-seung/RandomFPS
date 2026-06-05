// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuController.generated.h"

class UMainMenuUI;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API AMenuController : public APlayerController
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuUI> BP_MainMenuUI;
	UMainMenuUI* MainMenuUI;

private:
	virtual void BeginPlay() override;
};
