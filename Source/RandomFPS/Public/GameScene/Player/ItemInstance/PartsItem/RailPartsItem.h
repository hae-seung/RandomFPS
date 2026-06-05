// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "RailPartsItem.generated.h"

class URailPartsData;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API URailPartsItem : public UPartsItem
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	URailPartsData* RailPartsData;
	
public:
	virtual void Init(UItemData* Data) override;
	virtual UItemInstance* CloneItem() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
