// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/StructHeader.h"
#include "GunAbilityModifier.generated.h"

/**
 * 
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class RANDOMFPS_API UGunAbilityModifier : public UObject
{
	GENERATED_BODY()


public:
	virtual void ApplyGunAbility(FGunAbility* GunAbility) PURE_VIRTUAL(UGunAbilityModifier::ApplyGunAbility, );
	virtual FText GetAbilityDescription() PURE_VIRTUAL(UGunAbilityModifier::GetAbilityDescription, return FText::GetEmpty(););
};
