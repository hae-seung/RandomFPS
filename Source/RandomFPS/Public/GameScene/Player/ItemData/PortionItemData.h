// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "PortionItemData.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UPortionItemData : public UItemData
{
	GENERATED_BODY()

public:
	virtual UItemInstance* CreateItem(UObject* Outer) override;
	
};
