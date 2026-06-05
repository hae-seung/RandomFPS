// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/FireMode/FireData.h"

#include "GameScene/Player/ItemData/BulletItemData.h"
#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Weapon/FireMode/FireMode.h"


UFireMode* UFireData::GetFireMode(UGunItem* GunItem)
{
	UFireMode* NewFireMode = NewObject<UFireMode>(GunItem, FireMode.Get());
	NewFireMode->Init(GunItem->GunItemData->NormalTrueDamage, this);
	
	return NewFireMode;
}
