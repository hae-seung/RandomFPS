// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "MagazinePartsItem.generated.h"

class UMagazinePartsData;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UMagazinePartsItem : public UPartsItem
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	UMagazinePartsData* MagazinePartsData;
	

public:
	virtual void Init(UItemData* Data) override;
	virtual UItemInstance* CloneItem() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int GetAdditiveMaxMagAmmo();
};
