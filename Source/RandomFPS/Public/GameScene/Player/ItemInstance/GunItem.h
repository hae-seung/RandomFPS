// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/StructHeader.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "GunItem.generated.h"


class UFireMode;
class UPartsItem;
class UGunItemData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGunLevelChanged, int);
DECLARE_MULTICAST_DELEGATE(FOnGunAwake);


UCLASS()
class RANDOMFPS_API UGunItem : public UItemInstance
{
	GENERATED_BODY()

public:
	FOnGunLevelChanged OnGunLevelChanged;
	FOnGunAwake OnGunAwake;
	
public:
	UPROPERTY(Replicated)
	UGunItemData* GunItemData;
	
public:
	bool CheckPartsType(EPartsType PartsType)const;
	UPartsItem* GetCurrentEquipParts(EPartsType PartsType);
	void UnEquipParts(EPartsType PartsType);
	void EquipParts(UPartsItem* PartsItem, EPartsType PartsType);
	TObjectPtr<UFireMode> GetFireSystem(int SystemIndex);
	int GetMaxMagAmmo();
	
	float GetBulletSpeed() const;
	int GetGunLevel();
	const FGunAbilityWrapper* GetAbilityWrapper();
	bool IsAwake();
	void LevelUp();
	void AwakeGun();
	
public:
	virtual void Init(UItemData* Data) override;
	virtual UItemInstance* CloneItem() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
private:
	//장착 가능한지만 알 수 있게 파츠타입만 모아둠
	//UI때문에 Replicated 필요
	//2026-07-29 : 필요없음. Data값을 클라에서 그대로 UI에 쓰면 됨
	UPROPERTY(Replicated)
	TArray<EPartsType> PartsSet;

	//server
	//장착된 실제 아이템들
	UPROPERTY()
	UPartsItem* RailPartsItem;
	UPROPERTY()
	UPartsItem* MuzzlePartsItem;
	UPROPERTY()
	UPartsItem* MagazinePartsItem;

	UPROPERTY()
	TArray<TObjectPtr<UFireMode>> FireModes;

	

private:
	//총을 교체하게 될때 필요함
	int MaxMagAmmo;
	FGunAbility GunAbility;
	
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_GunLevel)
	int32 GunLevel;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_bAwake)
	bool bAwake;

private:
	void ModifyMaxMagAmount(bool bPlus);
	UFUNCTION()
	void OnRep_GunLevel();
	UFUNCTION()
	void OnRep_bAwake();

	void ApplyGunAbility();
	void ApplyAwakeGunAbility();
};

