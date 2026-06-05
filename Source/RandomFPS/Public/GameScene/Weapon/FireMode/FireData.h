// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FireData.generated.h"

class UGunItem;
class UFireMode;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UFireData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFireMode> FireMode;
	
public:
	virtual UFireMode* GetFireMode(UGunItem* GunItem);

private:
	//발사될 총알 수(점사, 샷건 진화용)
	UPROPERTY(EditAnywhere)
	int32 BulletCnt;
};
