// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "HasTargetDeco.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UHasTargetDeco : public UBTDecorator_Blackboard
{
	GENERATED_BODY()


protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
