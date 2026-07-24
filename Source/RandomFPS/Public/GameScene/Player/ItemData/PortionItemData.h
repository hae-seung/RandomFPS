// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/StructHeader.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "PortionItemData.generated.h"


UCLASS()
class RANDOMFPS_API UPortionItemData : public UItemData
{
	GENERATED_BODY()

public:
	virtual UItemInstance* CreateItem(UObject* Outer) override;
	const TArray<FStatModifier>& GetStatModifiers() const;
	float GetNeedTime();
	UAnimMontage* GetMontage();
	
private:
	//몇 초 뒤에 아이템이 실제로 사용될지
	UPROPERTY(EditAnywhere)
	float WaitTime;
	UPROPERTY(EditAnywhere)
	UAnimMontage* UsingMontage;
	
	//실제 사용되었을때 변할 능력치 모음
	UPROPERTY(EditAnywhere)
	TArray<FStatModifier> StatModifiers;
};
