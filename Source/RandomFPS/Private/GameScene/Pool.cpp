// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Pool.h"
#include "GameScene/Poolable.h"
#include "GameScene/Player/PlayerCharacter.h"


AActor* UPool::GetActor(TSubclassOf<AActor> BP_Actor, AActor* Player)
{
	if(PoolObjects.IsEmpty())
	{
		Spawn(BP_Actor);
	}

	AActor* LastActor = PoolObjects.Pop(EAllowShrinking::No);
	IPoolable* PoolActor = Cast<IPoolable>(LastActor);
	PoolActor->Acquire(Cast<APlayerCharacter>(Player));
	return LastActor;
}

void UPool::Spawn(TSubclassOf<AActor> BP_Actor)
{
	FActorSpawnParameters Params;
	Params.Owner = nullptr;
	
	AActor* NewActor = GetWorld()->SpawnActor<AActor>(BP_Actor, Params);
	IPoolable* PoolActor = Cast<IPoolable>(NewActor);
	PoolActor->Release();
	PoolObjects.Add(NewActor);
}
