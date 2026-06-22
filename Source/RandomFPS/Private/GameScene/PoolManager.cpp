// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/PoolManager.h"
#include "NiagaraFunctionLibrary.h"
#include "GameScene/Pool.h"


void UPoolManager::InitServerPool(const TArray<TSubclassOf<AActor>>& ServerPoolData)
{
	World = GetWorld();
	
	for(const auto& Value : ServerPoolData)
	{
		UPool* NewPool = NewObject<UPool>(this);
		ServerPool.Add(Value, NewPool);
	}
}


AActor* UPoolManager::Server_GetActor(TSubclassOf<AActor> BP_Actor, AActor* Owner)
{
	if(!ServerPool.Contains(BP_Actor))
	{
		UPool* NewPool = NewObject<UPool>(this);
		ServerPool.Add(BP_Actor, NewPool); 
	}
	
	return ServerPool[BP_Actor]->GetActor(BP_Actor, Owner);
}

void UPoolManager::Server_ReleaseActor(AActor* PoolingActor)
{
	if(!ServerPool.Contains(PoolingActor->GetClass()))
		return;
	
	ServerPool[PoolingActor->GetClass()]->ReleaseActor(PoolingActor);
}



UNiagaraComponent* UPoolManager::Client_PlayAttachedFX(
	UNiagaraSystem* FX, USceneComponent* AttachComp, const FName& SocketName)
{
	if(!FX || !AttachComp)
		return nullptr;

	//소켓 존재 여부 체크
	const bool bHasSocket = SocketName.IsNone() || AttachComp->DoesSocketExist(SocketName);

	const FName FinalSocketName = bHasSocket ? SocketName : NAME_None;

	return UNiagaraFunctionLibrary::SpawnSystemAttached(
		FX,
		AttachComp,
		FinalSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		FVector(1.f),
		EAttachLocation::SnapToTarget,
		true,
		ENCPoolMethod::AutoRelease,
		true,
		true
		);
}

UNiagaraComponent* UPoolManager::Client_PlayLocationFX(UNiagaraSystem* FX, const FVector& Location, const FRotator& Rot)
{
	if(!FX)
		return nullptr;

	return UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		FX,
		Location,
		Rot,
		FVector(0.4f),
		true,
		true,
		ENCPoolMethod::AutoRelease,
		true);
}


