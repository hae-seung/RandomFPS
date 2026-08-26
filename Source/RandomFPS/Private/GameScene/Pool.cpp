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
	
	LastActor->SetActorEnableCollision(true);
	LastActor->SetActorHiddenInGame(false);
	PoolActor->Acquire(Owner);

	UE_LOG(
	LogTemp,
	Warning,
	TEXT("[POOL GET] %s"),
	*GetNameSafe(LastActor)
);
	
	return LastActor;
}


void UPool::Spawn(TSubclassOf<AActor> BP_Actor)
{
	FActorSpawnParameters Params;
	Params.Owner = nullptr;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* NewActor = GetWorld()->SpawnActor<AActor>(BP_Actor, Params);
	PoolObjects.Add(NewActor);
}

void UPool::ReleaseActor(AActor* Object)
{
	UE_LOG(
	LogTemp,
	Warning,
	TEXT("[POOL RELEASE] %s"),
	*GetNameSafe(Object)
);

	Object->SetActorEnableCollision(false);
	Object->SetActorHiddenInGame(true);
	
	PoolObjects.Add(Object);
}

