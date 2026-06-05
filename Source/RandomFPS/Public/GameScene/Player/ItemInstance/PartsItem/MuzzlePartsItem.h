// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "MuzzlePartsItem.generated.h"

class UMuzzlePartsData;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UMuzzlePartsItem : public UPartsItem
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	UMuzzlePartsData* MuzzlePartsData;

public:
	virtual void Init(UItemData* Data) override;
	virtual UItemInstance* CloneItem() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
