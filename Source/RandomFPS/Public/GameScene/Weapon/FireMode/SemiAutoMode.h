// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireMode.h"
#include "UObject/NoExportTypes.h"
#include "SemiAutoMode.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API USemiAutoMode : public UFireMode
{
	GENERATED_BODY()

public:
	//virtual void Init(int TrueDamage, UFireData* FireData) override;
	
	virtual void Shot(TSubclassOf<ABullet> BP_Bullet,
		AMyPlayerController* Controller,
		AGun* GunActor,
		bool HasBullet) override;
};
