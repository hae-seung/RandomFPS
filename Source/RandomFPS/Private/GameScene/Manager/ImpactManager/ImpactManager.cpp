// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Manager/ImpactManager/ImpactManager.h"

#include "GameScene/PoolManager.h"
#include "GameScene/Manager/SoundManager/SoundManager.h"

void UImpactManager::InitImpactData(TMap<TEnumAsByte<EPhysicalSurface>, FImpactData>& ImpactData)
{
	BulletImpact = ImpactData;
}

void UImpactManager::SpawnImpact(EPhysicalSurface Surface, FVector Location, FRotator Rot)
{
	FImpactData* Data = BulletImpact.Find(Surface);
	if(!Data)
	{
		return;
	}

	//나이아가라 스폰
	if(!PoolManager)
	{
		PoolManager = GetWorld()->GetSubsystem<UPoolManager>();
	}
	PoolManager->Client_PlayLocationFX(Data->ImpactFX, Location, Rot);
	
	//사운드 재생
	if(!SoundManager)
	{
		SoundManager = GetWorld()->GetSubsystem<USoundManager>();
	}
	
	SoundManager->PlaySoundAtLocation(
		Data->HitSounds[FMath::RandRange(0, Data->HitSounds.Num() - 1)],
		Location);
}
