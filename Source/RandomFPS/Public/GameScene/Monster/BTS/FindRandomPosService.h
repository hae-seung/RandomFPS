// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FindRandomPosService.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UFindRandomPosService : public UBTService
{
	GENERATED_BODY()


protected:
	UFindRandomPosService();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	float Radius;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MoveLocation;
};
