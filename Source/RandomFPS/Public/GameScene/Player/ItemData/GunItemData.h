// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "GunItemData.generated.h"


class UBulletItemData;
class UNiagaraSystem;
class ABullet;
class UFireData;
class AGun;

UCLASS()
class RANDOMFPS_API UGunItemData : public UItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> BP_Gun;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> BP_GunAnimLayer;
	UPROPERTY(EditAnywhere)
	USkeletalMesh* GunSkin;
	UPROPERTY(EditAnywhere)
	TSet<EPartsType> PartsSet;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UFireData>> FireData;

	
	UPROPERTY(EditAnywhere)
	UBulletItemData* BulletItemData;
	UPROPERTY(EditAnywhere)
	int32 MaxMagAmount;
	UPROPERTY(EditAnywhere)
	int32 BulletSpeed;
	UPROPERTY(EditAnywhere)
	int32 NormalTrueDamage;

	
	//fx
	UPROPERTY(EditAnywhere)
	UAnimMontage* ShotMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* MuzzleFlashFX;

	//sfx
	UPROPERTY(EditAnywhere)
	USoundBase* EquipSFX;
	UPROPERTY(EditAnywhere)
	USoundBase* ShotSFX;
	
public:
	virtual UItemInstance* CreateItem(UObject* Outer) override;
};
