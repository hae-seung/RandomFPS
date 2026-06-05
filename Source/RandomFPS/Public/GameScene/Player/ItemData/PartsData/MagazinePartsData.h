// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemData/PartsItemData.h"
#include "MagazinePartsData.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UMagazinePartsData : public UPartsItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 MaxMagAmmo;

public:
	virtual UItemInstance* CreateItem(UObject* Outer) override;
};
