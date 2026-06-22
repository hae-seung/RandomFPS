// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Pool.h"
#include "GameScene/Poolable.h"
#include "GameScene/Player/PlayerCharacter.h"


AActor* UPool::GetActor(TSubclassOf<AActor> BP_Actor, AActor* Owner)
{
	if(PoolObjects.IsEmpty())
	{
		Spawn(BP_Actor);
	}

	AActor* LastActor = PoolObjects.Pop(EAllowShrinking::No);
	IPoolable* PoolActor = Cast<IPoolable>(LastActor);
	PoolActor->Acquire(Owner);
	return LastActor;
}


void UPool::Spawn(TSubclassOf<AActor> BP_Actor)
{
	FActorSpawnParameters Params;
	Params.Owner = nullptr;
	
	AActor* NewActor = GetWorld()->SpawnActor<AActor>(BP_Actor, Params);
	PoolObjects.Add(NewActor);
}

void UPool::ReleaseActor(AActor* Object)
{
	PoolObjects.Add(Object);
}

