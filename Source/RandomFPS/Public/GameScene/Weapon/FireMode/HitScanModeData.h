// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Weapon/FireMode/FireData.h"
#include "HitScanModeData.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UHitScanModeData : public UFireData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UMaterialInstance* Decal_BulletHole;
	UPROPERTY(EditAnywhere)
	FVector DecalSize;
	UPROPERTY(EditAnywhere)
	float DecalLifeSpan;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HitScanSmoke;
	UPROPERTY(EditAnywhere)
	FName StartName;
	UPROPERTY(EditAnywhere)
	FName EndName;

public:
	virtual UFireMode* GetFireMode(UGunItem* GunItem) override;
};
