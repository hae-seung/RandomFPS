// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/GunItem.h"

#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemData/PartsData/MagazinePartsData.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "GameScene/Player/ItemInstance/PartsItem/MagazinePartsItem.h"
#include "GameScene/Weapon/FireMode/FireData.h"
#include "Net/UnrealNetwork.h"


void UGunItem::Init(UItemData* Data)
{
	Super::Init(Data);
	GunItemData = Cast<UGunItemData>(Data);

	for(auto Value : GunItemData->PartsSet)
	{
		PartsSet.Add(Value);
	}

	for(auto Value : GunItemData->FireData)
	{
		FireModes.Add(Value->GetFireMode(this));
	}

	BulletSpeed = GunItemData->BulletSpeed;
	MaxMagAmmo = GunItemData->MaxMagAmount;
}

UItemInstance* UGunItem::CloneItem()
{
	UGunItem* NewItem = NewObject<UGunItem>(GetOuter());
	NewItem->Init(GunItemData);
	return NewItem;
}

void UGunItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGunItem, GunItemData);
	DOREPLIFETIME(UGunItem, PartsSet);
}

int UGunItem::GetBulletSpeed() const
{
	//임시
	return BulletSpeed;
}

bool UGunItem::CheckPartsType(EPartsType PartsType) const
{
	if(PartsSet.Contains(PartsType))
		return true;

	return false;
}

UPartsItem* UGunItem::GetCurrentEquipParts(EPartsType PartsType)
{
	UPartsItem* PartsItem = nullptr;
	switch (PartsType)
	{
	case EPartsType::Rail:
		PartsItem = RailPartsItem;
		break;
	case EPartsType::Muzzle:
		PartsItem = MuzzlePartsItem;
		break;
	case EPartsType::Magazine:
		PartsItem = MagazinePartsItem;
		break;
	}

	return PartsItem;
}

void UGunItem::EquipParts(UPartsItem* PartsItem, EPartsType PartsType)
{
	switch (PartsType)
	{
	case EPartsType::Rail:
		RailPartsItem = PartsItem;
		break;
	case EPartsType::Muzzle:
		MuzzlePartsItem = PartsItem;
		break;
	case EPartsType::Magazine:
		MagazinePartsItem = PartsItem;
		ModifyMaxMagAmount(true);
		break;
	}
}

void UGunItem::UnEquipParts(EPartsType PartsType)
{
	switch (PartsType)
	{
	case EPartsType::Rail:
		RailPartsItem = nullptr;
		break;
	case EPartsType::Muzzle:
		MuzzlePartsItem = nullptr;
		break;
	case EPartsType::Magazine:
		ModifyMaxMagAmount(false);
		MagazinePartsItem = nullptr;
		break;
	}
}


TObjectPtr<UFireMode> UGunItem::GetFireSystem(int SystemIndex)
{
	if(SystemIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Worng Index"));
		return nullptr;
	}
	
	if(FireModes.Num() == 0)
	{
		for(auto Value : GunItemData->FireData)
		{
			FireModes.Add(Value->GetFireMode(this));
		}
	}

	return FireModes[SystemIndex];
}

int UGunItem::GetMaxMagAmmo()
{
	return MaxMagAmmo;
}

void UGunItem::ModifyMaxMagAmount(bool bPlus)
{
	if(UMagazinePartsItem* MagItem = Cast<UMagazinePartsItem>(MagazinePartsItem))
	{
		int AdditiveMaxMagAmmo = MagItem->GetAdditiveMaxMagAmmo();

		if(bPlus)
		{
			MaxMagAmmo += AdditiveMaxMagAmmo;
		}
		else
		{
			MaxMagAmmo -= AdditiveMaxMagAmmo;
		}
	}
}