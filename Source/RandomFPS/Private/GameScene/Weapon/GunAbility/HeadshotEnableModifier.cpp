// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/GunAbility/HeadshotEnableModifier.h"

void UHeadshotEnableModifier::ApplyGunAbility(FGunAbility* GunAbility)
{
	GunAbility->bCanHeadShot = bCanHeadShot;
}

FText UHeadshotEnableModifier::GetAbilityDescription()
{
	if(bCanHeadShot)
	{
		return FText::FromString(TEXT("헤드샷 가능"));
	}
	else
	{
		return FText::FromString(TEXT("헤드샷 불가능"));
	}
}
