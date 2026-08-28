// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/StructHeader.h"
#include "GameScene/Cards/CardAbility/Base/CardAbility.h"
#include "MaxHpModifierAbility.generated.h"


UCLASS()
class RANDOMFPS_API UMaxHpModifierAbility : public UCardAbility
{
	GENERATED_BODY()

public:
	virtual UCardAbility* Clone() override;
	virtual void OnAcquire(APlayerCharacter* Player) override;
	virtual void OnRemove(APlayerCharacter* Player) override;


private:
	UPROPERTY(EditAnywhere)
	TArray<FStatModifier> StatModifier;
};
