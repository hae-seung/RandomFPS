// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UpdateTargetService.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API UUpdateTargetService : public UBTService
{
	GENERATED_BODY()


protected:
	UUpdateTargetService();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;
};
