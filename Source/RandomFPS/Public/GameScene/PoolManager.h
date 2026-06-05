// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NiagaraSystem.h"
#include "PoolManager.generated.h"

class UPool;


UCLASS()
class RANDOMFPS_API UPoolManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void InitServerPool(const TArray<TSubclassOf<AActor>>& ServerPoolData);

	AActor* Server_GetActor(TSubclassOf<AActor> BP_Actor, AActor* Player);
	UNiagaraComponent* Client_PlayAttachedFX(UNiagaraSystem* FX, USceneComponent* AttachComp, const FName& SocketName);
	UNiagaraComponent* Client_PlayLocationFX(UNiagaraSystem* FX, const FVector& Location, const FRotator& Rot);
	
private:
	UPROPERTY()
	UWorld* World;
	
	UPROPERTY()
	TMap<TSubclassOf<AActor>, TObjectPtr<UPool>> ServerPool;
	
};
