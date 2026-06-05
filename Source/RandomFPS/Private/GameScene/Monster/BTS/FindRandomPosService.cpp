// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/BTS/FindRandomPosService.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UFindRandomPosService::UFindRandomPosService()
{
	bNotifyBecomeRelevant = true;
}

void UFindRandomPosService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	const APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	const FVector CurPos = Pawn->GetActorLocation();

	const UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
	if(!Nav)
		return;

	FNavLocation RandomLocation;
	bool bFound = Nav->GetRandomPointInNavigableRadius(
		CurPos,
		Radius,
		RandomLocation);
	
	if(bFound)
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		BB->SetValueAsVector(MoveLocation.SelectedKeyName, RandomLocation.Location);
	}
}
