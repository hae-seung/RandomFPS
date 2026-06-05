// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "MoveLastLocationTask.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UMoveLastLocationTask : public UBTTask_MoveTo
{
	GENERATED_BODY()


protected:
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector bLostTarget;
};
