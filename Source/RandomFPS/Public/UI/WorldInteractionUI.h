// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldInteractionUI.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UWorldInteractionUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Open();
	void Close();

protected:
	UPROPERTY()
	APlayerCharacter* APC;
	
protected:
	virtual void NativeOnInitialized() override;
};
