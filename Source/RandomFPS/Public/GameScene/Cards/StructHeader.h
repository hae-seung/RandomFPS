// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "StructHeader.generated.h"

class UBulletItemData;
class APlayerCharacter;
class UHitScanModeData;
class UNiagaraSystem;

USTRUCT()
struct FCardEventContext
{
	GENERATED_BODY()

public:
	UPROPERTY()
	APlayerCharacter* Player;

	UPROPERTY()
	ECardTrigger Trigger;

	UPROPERTY()
	float Value;
};

//운영자모드 => UI에서 총알 할당용
USTRUCT(BlueprintType) 
struct FBulletService
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UBulletItemData* BulletItemData;
	UPROPERTY(EditAnywhere)
	int32 Amount;
};

USTRUCT()
struct FHitScanInfo
{
	GENERATED_BODY()

	UPROPERTY()
	UNiagaraSystem* HitScanEffect;
	UPROPERTY()
	FVector StartLoc;
	UPROPERTY()
	FVector EndLoc;
	UPROPERTY()
	FName StartName;
	UPROPERTY()
	FName EndName;
};

USTRUCT()
struct FHitScanImpactInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FVector HitLoc;
	UPROPERTY()
	FRotator DecalRot;
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> Surface;
	UPROPERTY()
	FVector DecalSize;
	UPROPERTY()
	float DecalLifeSpan;
	UPROPERTY()
	UMaterialInterface* DecalMaterial;
};

USTRUCT(BlueprintType)
struct FAISightPerceptionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float SightRadius = 1500.f;
	UPROPERTY(EditAnywhere)
	float LoseSightRadius = 1800.f;
	UPROPERTY(EditAnywhere)
	float SightAngle = 70.f;
};
