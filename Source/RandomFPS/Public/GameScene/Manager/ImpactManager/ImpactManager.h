// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Struct/ImpactData.h"
#include "Subsystems/WorldSubsystem.h"
#include "ImpactManager.generated.h"

class USoundManager;
class UPoolManager;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UImpactManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void InitImpactData(TMap<TEnumAsByte<EPhysicalSurface>, FImpactData>& ImpactData);
	void SpawnImpact(EPhysicalSurface Surface, FVector Location, FRotator Rot);

private:
	UPROPERTY()
	TMap<TEnumAsByte<EPhysicalSurface>, FImpactData> BulletImpact;

	UPROPERTY()
	UPoolManager* PoolManager;
	UPROPERTY()
	USoundManager* SoundManager;
};
