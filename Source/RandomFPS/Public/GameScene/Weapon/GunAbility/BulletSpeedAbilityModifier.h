// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Weapon/GunAbility/GunAbilityModifier.h"
#include "BulletSpeedAbilityModifier.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UBulletSpeedAbilityModifier : public UGunAbilityModifier
{
	GENERATED_BODY()

public:
	virtual void ApplyGunAbility(FGunAbility* GunAbility) override;
	virtual FText GetAbilityDescription() override;

private:
	UPROPERTY(EditAnywhere)
	float BulletSpeed;
};
