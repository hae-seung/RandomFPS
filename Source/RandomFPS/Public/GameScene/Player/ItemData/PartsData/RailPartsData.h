// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemData/PartsItemData.h"
#include "RailPartsData.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API URailPartsData : public UPartsItemData
{
	GENERATED_BODY()

public:
	FORCEINLINE FName GetCameraSocketName() const { return CameraSocketName; }
	virtual UItemInstance* CreateItem(UObject* Outer) override;

	
protected:
	//상단 파츠가 붙으면서 옮겨질 카메라 위치
	UPROPERTY(EditAnywhere)
	FName CameraSocketName;
};
