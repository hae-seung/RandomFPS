// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "ImpactData.generated.h"

//표면 재질에 따른 Impact데이터
USTRUCT(Blueprintable)
struct FImpactData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<USoundBase>> HitSounds;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ImpactFX;
};

