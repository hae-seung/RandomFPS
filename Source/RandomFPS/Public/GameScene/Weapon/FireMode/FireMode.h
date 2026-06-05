// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireMode.generated.h"

class UFireData;
class AMyPlayerController;
class AGun;
class ABullet;


UCLASS(Abstract)
class RANDOMFPS_API UFireMode : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(int TrueDamage, UFireData* FireData);

	virtual void Shot(TSubclassOf<ABullet> BP_Bullet,
		AMyPlayerController* Controller,
		AGun* GunActor,
		bool HasBullet = false);
		
	virtual void StopShot();

protected:
	bool bInit = false;
	bool IsShooting = false;
	int32 NormalTrueDamage;
	
	FHitResult Hit;
	FVector MuzzleLoc;
	FVector TargetPoint;
	FVector ShootDir;
	
	
	
private:
	virtual bool IsSupportedForNetworking() const override;
	UFireData* Data;
};
