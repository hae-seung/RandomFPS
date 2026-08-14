// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/GunAbility/ReinforceData.h"

const FGunAbilityWrapper* UReinforceData::GetAbility(int Level)
{
	if(Level < 0 || Level > AbilityWrappers.Num())
	{
		return nullptr;
	}
	
	return &AbilityWrappers[Level];
}

const TArray<TObjectPtr<UGunAbilityModifier>>* UReinforceData::GetAwakeAbility()
{
	if(AwakeAbilityWrapper.Num() == 0)
		return nullptr;
	
	return &AwakeAbilityWrapper;
}

