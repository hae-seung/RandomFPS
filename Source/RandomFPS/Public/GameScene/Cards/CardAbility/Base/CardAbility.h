// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CardAbility.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class RANDOMFPS_API UCardAbility : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnAcquire(APlayerCharacter* Player) {}
	virtual void OnRemove(APlayerCharacter* Player) {}
};
