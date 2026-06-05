// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTD/BoolConditionDeco.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UBoolConditionDeco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	return BB->GetValueAsBool(BlackboardKey.SelectedKeyName);
}
