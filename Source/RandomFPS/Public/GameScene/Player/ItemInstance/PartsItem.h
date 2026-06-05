// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "PartsItem.generated.h"

class UPartsItemData;
/**
 * 
 */
UCLASS(Abstract)
class RANDOMFPS_API UPartsItem : public UItemInstance
{
	GENERATED_BODY()

public:
	EPartsType GetPartsType() const;
	FORCEINLINE UPartsItemData* GetPartsItemData() const { return PartsItemData; }
	
	virtual void Init(UItemData* Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(Replicated)
	UPartsItemData* PartsItemData;
};
