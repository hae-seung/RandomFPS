// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "BulletItem.generated.h"

class UBulletItemData;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UBulletItem : public UItemInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	UBulletItemData* BulletItemData;

public:
	virtual void Init(UItemData* Data) override;
	virtual UItemInstance* CloneItem() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
