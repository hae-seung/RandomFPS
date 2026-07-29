// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameScene/Cards/StructHeader.h"
#include "ReinforceData.generated.h"



UCLASS()
class RANDOMFPS_API UReinforceData : public UDataAsset
{
	GENERATED_BODY()

public:
	const FGunAbilityWrapper* GetAbility(int Level);

private:
	UPROPERTY(EditAnywhere)
	TArray<FGunAbilityWrapper> AbilityWrappers;
};
