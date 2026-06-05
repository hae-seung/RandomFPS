// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Pool.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UPool : public UObject
{
	GENERATED_BODY()

public:
	AActor* GetActor(TSubclassOf<AActor> BP_Actor, AActor* Player);

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> PoolObjects;

private:
	void Spawn(TSubclassOf<AActor> BP);
};
