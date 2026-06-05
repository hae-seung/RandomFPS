// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "GunItem.generated.h"


class UFireMode;
class UPartsItem;
class UGunItemData;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UGunItem : public UItemInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	UGunItemData* GunItemData;
	bool CheckPartsType(EPartsType PartsType)const;
	UPartsItem* GetCurrentEquipParts(EPartsType PartsType);
	void UnEquipParts(EPartsType PartsType);
	void EquipParts(UPartsItem* PartsItem, EPartsType PartsType);
	TObjectPtr<UFireMode> GetFireSystem(int SystemIndex);
	int GetMaxMagAmmo();
	
	int GetBulletSpeed() const;
	
public:
	virtual void Init(UItemData* Data) override;
	virtual UItemInstance* CloneItem() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
private:
	//장착 가능한지만 알 수 있게 파츠타입만 모아둠
	//UI때문에 Replicated 필요
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
	
	TArray<TObjectPtr<UFireMode>> FireModes;
	

private:
	//GunInstance Data
	int BulletSpeed;
	int MaxMagAmmo;

private:
	void ModifyMaxMagAmount(bool bPlus);
};

