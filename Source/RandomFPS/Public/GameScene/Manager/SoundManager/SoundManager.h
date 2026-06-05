// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API USoundManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void PlaySound2D(USoundBase* Sound);
	UAudioComponent* PlayAttachedSound(USoundBase* Sound, USceneComponent* MeshComp);
	void PlaySoundAtLocation(USoundBase* Sound, const FVector& Location);
};
