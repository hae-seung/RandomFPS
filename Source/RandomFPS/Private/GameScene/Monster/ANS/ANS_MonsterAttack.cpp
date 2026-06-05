// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/ANS/ANS_MonsterAttack.h"

#include "GameScene/Monster/Monster.h"


void UANS_MonsterAttack::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if(AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		Monster->StartAttack();
	}
}

void UANS_MonsterAttack::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if(!bUseTick)
		return;

	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_MonsterAttack::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner()))
	{
		Monster->EndAttack();
	}
}

