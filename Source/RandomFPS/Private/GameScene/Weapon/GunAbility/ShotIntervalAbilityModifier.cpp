// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/GunAbility/ShotIntervalAbilityModifier.h"

void UShotIntervalAbilityModifier::ApplyGunAbility(FGunAbility* GunAbility)
{
	GunAbility->ShotInterval = ShotInterval;
}

FText UShotIntervalAbilityModifier::GetAbilityDescription()
{
	return FText::Format(
		FText::FromString(TEXT("발사 간격 : {0}초")),
		ShotInterval
		);
}
