// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Manager/ObjectPool/PoolActor.h"

APoolActor::APoolActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APoolActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APoolActor::Acquire()
{
	
}

void APoolActor::Release()
{
}

bool APoolActor::IsActive() const
{
	return bIsActive;
}



