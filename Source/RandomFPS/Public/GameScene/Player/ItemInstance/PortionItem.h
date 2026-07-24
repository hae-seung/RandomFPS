// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "Interface/Useable.h"
#include "PortionItem.generated.h"

class UPortionItemData;


UCLASS()
class RANDOMFPS_API UPortionItem :
	public UItemInstance,
	public IUseable
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	UPortionItemData* PortionItemData;
	
public:
	virtual void Init(UItemData* Data) override;
	virtual UItemInstance* CloneItem() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	virtual float GetNeedTime() override;
	virtual void Use(UPlayerStatSystem* PlayerStatSystem) override;
	virtual UAnimMontage* GetUsingMontage() override;
};
