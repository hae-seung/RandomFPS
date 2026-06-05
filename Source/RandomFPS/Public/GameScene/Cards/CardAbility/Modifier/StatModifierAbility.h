// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/CardAbility/Base/ModifierCardAbility.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "StatModifierAbility.generated.h"


UCLASS()
class RANDOMFPS_API UStatModifierAbility : public UModifierCardAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EStat Stat;
	
	UPROPERTY(EditAnywhere)
	float Value;

	virtual void OnAcquire(APlayerCharacter* Player) override;
	virtual void OnRemove(APlayerCharacter* Player) override;
};
