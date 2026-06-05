// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireMode.h"
#include "FullAutoMode.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UFullAutoMode : public UFireMode
{
	GENERATED_BODY()

public:
	virtual void Shot(
		TSubclassOf<ABullet> BP_Bullet,
		AMyPlayerController* Controller,
		AGun* GunActor,
		bool HasBullet) override;

	virtual void StopShot() override;


private:
	UPROPERTY()
	FTimerHandle ShotTimer;
	UPROPERTY()
	TSubclassOf<ABullet> CachedBP_Bullet;
	UPROPERTY()
	TObjectPtr<AMyPlayerController> CachedController;
	UPROPERTY()
	TObjectPtr<AGun> CachedGunActor;
	bool CachedHasBullet;

	//todo : 강화 가능할거임
	float FireBetTime = 0.15f;
	
private:
	UFUNCTION()
	void AutoShot();
};
