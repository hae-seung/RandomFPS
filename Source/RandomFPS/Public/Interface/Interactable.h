// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class APlayerCharacter;
class UCameraComponent;
class UPlayerInteractSystem;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};


class RANDOMFPS_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void SetInteractState(bool bState) = 0;
	virtual void Interact(APlayerCharacter* APC) = 0;
	virtual void StopInteract(APlayerCharacter* APC) = 0;
};
