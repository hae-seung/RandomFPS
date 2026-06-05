// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTT/RotateRandomTask.h"

#include "AIController.h"

EBTNodeResult::Type URotateRandomTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CurrentRot = OwnerComp.GetAIOwner()->GetPawn()->GetActorRotation();
	TargetRotator = FRotator(0.f, FMath::RandRange(0,360), 0.f);
	ElapsedTime = 0.f;
	
	InterpSpeed = FMath::RandRange(MinInterpSpeed, MaxInterpSpeed);
	Duration = FMath::RandRange(MinDuration, MaxDuration);

	return EBTNodeResult::InProgress;
}

void URotateRandomTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	CurrentRot = FMath::RInterpTo(
		CurrentRot,
		TargetRotator,
		DeltaSeconds,
		InterpSpeed);

	OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(CurrentRot);

	ElapsedTime += DeltaSeconds;
	if(ElapsedTime >= Duration)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
