// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTD/HasTargetDeco.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UHasTargetDeco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if(BB->GetValueAsObject(BlackboardKey.SelectedKeyName) != nullptr)
		return true;

	return false;
}
