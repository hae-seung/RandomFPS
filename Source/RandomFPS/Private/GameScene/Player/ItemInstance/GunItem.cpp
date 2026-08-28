// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ItemInstance/GunItem.h"

#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemData/PartsData/MagazinePartsData.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "GameScene/Player/ItemInstance/PartsItem/MagazinePartsItem.h"
#include "GameScene/Weapon/FireMode/FireData.h"
#include "GameScene/Weapon/GunAbility/GunAbilityModifier.h"
#include "GameScene/Weapon/GunAbility/ReinforceData.h"
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

	ApplyGunAbility();
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
	DOREPLIFETIME(UGunItem, GunLevel);
	DOREPLIFETIME(UGunItem, bAwake);
}

float UGunItem::GetBulletSpeed() const
{
	return GunAbility.BulletSpeed;
}

int UGunItem::GetGunLevel()
{
	return GunLevel;
}

const FGunAbilityWrapper* UGunItem::GetAbilityWrapper(int InGunLevel)
{
	return GunItemData->ReinforceData->GetAbility(InGunLevel);
}

const TArray<TObjectPtr<UGunAbilityModifier>>* UGunItem::GetAwakeAbility()
{
	return GunItemData->ReinforceData->GetAwakeAbility();
}


bool UGunItem::IsAwake()
{
	return bAwake;
}

void UGunItem::LevelUp()
{
	if(GunLevel >= 5)
	{
		AwakeGun();
		return;
	}

	GunLevel++;
	
	OnGunLevelChanged.Broadcast(GunLevel);

	ApplyGunAbility();
}
void UGunItem::AwakeGun()
{
	if(GunLevel < 5 || bAwake)
		return;

	bAwake = true;
	OnGunAwake.Broadcast();
	ApplyAwakeGunAbility();
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

void UGunItem::ApplyGunAbility()
{
	const FGunAbilityWrapper* CurrentLevelWrapper = GetAbilityWrapper(GunLevel);
	if(!CurrentLevelWrapper)
		return;
	
	for(auto Modifier : CurrentLevelWrapper->GunAbilityModifiers)
	{
		Modifier->ApplyGunAbility(&GunAbility);
	}
}

void UGunItem::ApplyAwakeGunAbility()
{
	const TArray<TObjectPtr<UGunAbilityModifier>>* AwakeAbilities = GunItemData->ReinforceData->GetAwakeAbility();
	if(AwakeAbilities == nullptr)
		return;

	for(auto Modifier : *AwakeAbilities)
	{
		Modifier->ApplyGunAbility(&GunAbility);
	}
}


void UGunItem::OnRep_GunLevel()
{
	OnGunLevelChanged.Broadcast(GunLevel);
}
void UGunItem::OnRep_bAwake()
{
	OnGunAwake.Broadcast();
}
