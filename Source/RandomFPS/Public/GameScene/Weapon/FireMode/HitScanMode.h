// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireMode.h"
#include "GameScene/Weapon/Gun.h"
#include "HitScanMode.generated.h"

class UHitScanModeData;


UCLASS()
class RANDOMFPS_API UHitScanMode : public UFireMode
{
	GENERATED_BODY()

public:
	virtual void Init(int TrueDamage, UFireData* FireData) override;
	
	virtual void Shot(
		TSubclassOf<ABullet> BP_Bullet,
		AMyPlayerController* Controller,
		AGun* GunActor,
		bool HasBullet) override;

private:
	UPROPERTY()
	UHitScanModeData* HitScanData;

private:
	void SpawnHitImpact(AGun* GunActor);
};
