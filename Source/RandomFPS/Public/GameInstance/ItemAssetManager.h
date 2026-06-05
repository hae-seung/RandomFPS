// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ItemAssetManager.generated.h"


class UItemData;



UCLASS()
class RANDOMFPS_API UItemAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static const FPrimaryAssetType ItemDataType;
	
public:
	static UItemAssetManager& Get();
	UItemData* GetItemData(FName ItemId) const;
	
};
