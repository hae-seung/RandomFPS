// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/FireMode/HitScanModeData.h"

#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Weapon/FireMode/HitScanMode.h"

UFireMode* UHitScanModeData::GetFireMode(UGunItem* GunItem)
{
	UHitScanMode* HitScanMode = NewObject<UHitScanMode>(GunItem, FireMode.Get());
	HitScanMode->Init(GunItem->GunItemData->NormalTrueDamage, this);
	
	return HitScanMode;
}
