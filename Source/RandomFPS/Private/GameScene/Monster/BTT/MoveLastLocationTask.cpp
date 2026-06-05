// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTT/MoveLastLocationTask.h"

void UMoveLastLocationTask::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if(BB)
	{
		BB->SetValueAsBool(bLostTarget.SelectedKeyName, false);
	}

	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
