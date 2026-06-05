// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemData/PartsItemData.h"
#include "MuzzlePartsData.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UMuzzlePartsData : public UPartsItemData
{
	GENERATED_BODY()

public:
	virtual UItemInstance* CreateItem(UObject* Outer) override;
};
