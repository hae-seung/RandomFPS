// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolActor.generated.h"

UCLASS()
class RANDOMFPS_API APoolActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APoolActor();

protected:
	virtual void BeginPlay() override;

protected:
	bool bIsActive;
	
public:	
	virtual void Acquire();
	virtual void Release();
	virtual bool IsActive() const;

};
