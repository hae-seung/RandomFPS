// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTS/UpdateTargetService.h"

#include "GameScene/Monster/MonsterController.h"


UUpdateTargetService::UUpdateTargetService()
{
	bNotifyTick = true;
}

void UUpdateTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(AMonsterController* AMC = Cast<AMonsterController>(OwnerComp.GetAIOwner()))
	{
		//쫓아가면서 수시로 공격가능한지 체크 요청
		AMC->CheckCanAttackTarget();
	}
}

