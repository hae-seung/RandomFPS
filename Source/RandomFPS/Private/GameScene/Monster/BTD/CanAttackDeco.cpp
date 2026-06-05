// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTD/CanAttackDeco.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameScene/Monster/MonsterController.h"

bool UCanAttackDeco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//진입 조건은 BlackBoard의 CanAttack Key = true

	if(AMonsterController* AMC = Cast<AMonsterController>(OwnerComp.GetAIOwner()))
	{
		//CanAttack인지 몬스터 쪽에서 다시 한 번 확인하고 덮어씀.
		
		//공격 몽타주 재생도중에 타겟이 범위 밖으로 도망갔을 때
		//CanAttack을 다시 한번 더 Update해줘야함.
		//안그러면 CanAttack = true인 상태로 제자리에서 계속 공격만 할거임.
		AMC->CheckCanAttackTarget();
	}
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	return BB->GetValueAsBool(BlackboardKey.SelectedKeyName);
}
