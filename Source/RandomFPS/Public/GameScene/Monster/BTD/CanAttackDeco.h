// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "CanAttackDeco.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UCanAttackDeco : public UBTDecorator_Blackboard
{
	GENERATED_BODY()


protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
