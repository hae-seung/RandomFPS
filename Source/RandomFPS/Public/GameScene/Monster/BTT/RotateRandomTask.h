// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RotateRandomTask.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API URotateRandomTask : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	UPROPERTY()
	FRotator TargetRotator;
	UPROPERTY()
	FRotator CurrentRot;
	
	UPROPERTY(EditAnywhere)
	float MinInterpSpeed;
	UPROPERTY(EditAnywhere)
	float MaxInterpSpeed;
	UPROPERTY(EditAnywhere)
	float MinDuration;
	UPROPERTY(EditAnywhere)
	float MaxDuration;
	
	float ElapsedTime;
	float InterpSpeed;
	float Duration;
	
};
