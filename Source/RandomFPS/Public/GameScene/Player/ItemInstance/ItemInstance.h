// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.generated.h"


class UItemData;
class UItemInstance;

DECLARE_DELEGATE_OneParam(FOnItemStateChanged, int32)



UCLASS(Abstract)
class RANDOMFPS_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	FOnItemStateChanged OnItemStateChangedEvent;

	UPROPERTY(Replicated)
	UItemData* ItemData;
	
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_ItemAmount)
	int32 ItemAmount;

	
public:
	virtual void Init(UItemData* Data);
	bool IsMax();
	FName GetItemId();
	void SetAmount(int Amount);
	void SetSlotIndex(int Index);
	int32 GetMaxAmount();
	int32 AddAmountAndGetExcess(int32 Amount);
	virtual UItemInstance* CloneItem() PURE_VIRTUAL(UItemInstance::CloneItem, return nullptr; );

protected:
	int32 SlotIndex;
	
protected:
	
	//★네트워크 채널에 등록되기 위한 필수조건
	virtual bool IsSupportedForNetworking() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
private:
	UFUNCTION()
	void OnRep_ItemAmount();
};
