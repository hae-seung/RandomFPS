// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "BulletItemData.generated.h"

class ABullet;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UBulletItemData : public UItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABullet> BP_Bullet;
	

	
public:
	virtual UItemInstance* CreateItem(UObject* Outer) override;
};
