// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "PartsItemData.generated.h"

class AParts;
/**
 * 
 */
UCLASS(Abstract)
class RANDOMFPS_API UPartsItemData : public UItemData
{
	GENERATED_BODY()

public:
	FORCEINLINE FName GetPartsSocketName() const { return PartsSocketName; }
	FORCEINLINE EPartsType GetPartsType() const { return PartsType; }
	FORCEINLINE TSubclassOf<AParts> GetBP_Parts() const { return BP_Parts; }

	
protected:
	//총기에 부착될 소켓 이름
	UPROPERTY(EditAnywhere)
	FName PartsSocketName;

	UPROPERTY(EditAnywhere)
	EPartsType PartsType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AParts> BP_Parts;
	
};
