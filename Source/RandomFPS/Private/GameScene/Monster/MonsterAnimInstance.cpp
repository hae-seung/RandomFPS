// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/MonsterAnimInstance.h"

#include "GameScene/Monster/Monster.h"


void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Monster = Cast<AMonster>(TryGetPawnOwner());
}

void UMonsterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if(!Monster)
		return;

	Speed = Monster->GetVelocity().Size2D();
}
