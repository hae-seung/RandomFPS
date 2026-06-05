// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Struct/ImpactData.h"
#include "PoolInitActor.generated.h"

class UPoolData;

UCLASS()
class RANDOMFPS_API APoolInitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APoolInitActor();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ServerPoolData;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UNiagaraSystem>> ClientPool;
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, FImpactData> BulletImpactData;
};
