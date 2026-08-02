// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "GameFramework/Actor.h"
#include "Computer.generated.h"

class UBoxComponent;

UCLASS()
class RANDOMFPS_API AComputer : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	AComputer();

protected:
	virtual void BeginPlay() override;
};
