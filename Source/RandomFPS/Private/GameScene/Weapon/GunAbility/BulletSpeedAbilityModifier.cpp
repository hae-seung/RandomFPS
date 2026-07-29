// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/GunAbility/BulletSpeedAbilityModifier.h"

void UBulletSpeedAbilityModifier::ApplyGunAbility(FGunAbility* GunAbility)
{
	GunAbility->BulletSpeed = BulletSpeed;
}

FText UBulletSpeedAbilityModifier::GetAbilityDescription()
{
	return FText::Format(
		FText::FromString(TEXT("총알 속도 : {0} m/s")),
		BulletSpeed);
}
