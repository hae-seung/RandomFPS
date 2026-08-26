// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "RoundStruct.generated.h"

USTRUCT()
struct FRoundInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERoundType RoundType;
	UPROPERTY(EditAnywhere)
	int RoundGold;
	UPROPERTY(EditAnywhere)
	int RoundDeathLife;
	UPROPERTY(EditAnywhere)
	bool bGetCard;
	UPROPERTY(EditAnywhere)
	int RecoveryLife;
};
