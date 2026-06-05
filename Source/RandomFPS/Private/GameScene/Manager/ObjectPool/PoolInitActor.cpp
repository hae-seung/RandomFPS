// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Manager/ObjectPool/PoolInitActor.h"

#include "GameScene/PoolManager.h"
#include "GameScene/Manager/ImpactManager/ImpactManager.h"

APoolInitActor::APoolInitActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void APoolInitActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(UPoolManager* PoolManager = GetWorld()->GetSubsystem<UPoolManager>())
	{
		if(HasAuthority())
		{
			PoolManager->InitServerPool(ServerPoolData);
		}
	}

	if(UImpactManager* ImpactManager = GetWorld()->GetSubsystem<UImpactManager>())
	{
		//서버 클라 구분 없이 무조건 가지고 잇어야함
		ImpactManager->InitImpactData(BulletImpactData);
	}
}


