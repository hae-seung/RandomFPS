// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTNodeStructHeader.generated.h"

class UBlackboardComponent;


USTRUCT()
struct FBTS_UpdateTargetServiceNode
{
	GENERATED_BODY()

	UPROPERTY()
	UBlackboardComponent* BB;
	UPROPERTY()
	AActor* Target;
};
