// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTT/AttackTask.h"

#include "AIController.h"
#include "GameScene/Monster/MonsterController.h"

UAttackTask::UAttackTask()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(AMonsterController* AMC = Cast<AMonsterController>(OwnerComp.GetAIOwner()))
	{
		AMC->Attack();
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UAttackTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(AMonsterController* AMC = Cast<AMonsterController>(OwnerComp.GetAIOwner()))
	{
		if(!AMC->IsAttacking())
		{
			//공격이 끝났다면 공격 Task 종료
			FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		}
	}
}
