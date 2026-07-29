// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Weapon/GunAbility/GunAbilityModifier.h"
#include "HeadshotEnableModifier.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UHeadshotEnableModifier : public UGunAbilityModifier
{
	GENERATED_BODY()

public:
	virtual void ApplyGunAbility(FGunAbility* GunAbility) override;
	virtual FText GetAbilityDescription() override;

private:
	UPROPERTY(EditAnywhere)
	bool bCanHeadShot;
};
