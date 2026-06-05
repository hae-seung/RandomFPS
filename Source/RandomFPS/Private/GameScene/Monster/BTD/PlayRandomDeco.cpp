// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTD/PlayRandomDeco.h"

bool UPlayRandomDeco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(RandomProbability <= 0)
		return false;

	if(RandomProbability >= 100)
	{
		return true;
	}

	const int Roll = FMath::RandRange(1, 100);

	return RandomProbability >= Roll;
}
