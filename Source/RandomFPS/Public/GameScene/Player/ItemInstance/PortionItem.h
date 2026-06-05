// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "PortionItem.generated.h"

class UPortionItemData;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UPortionItem : public UItemInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	UPortionItemData* PortionItemData;
	
public:
	virtual void Init(UItemData* Data) override;
	virtual UItemInstance* CloneItem() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
