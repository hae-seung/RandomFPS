// Fill out your copyright notice in the Description page of Project Settings.

#include "GameScene/Manager/SoundManager/SoundManager.h"

#include "Kismet/GameplayStatics.h"

void USoundManager::PlaySound2D(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(this, Sound);
}

UAudioComponent* USoundManager::PlayAttachedSound(USoundBase* Sound, USceneComponent* MeshComp)
{
	return UGameplayStatics::SpawnSoundAttached(Sound, MeshComp);
}

void USoundManager::PlaySoundAtLocation(USoundBase* Sound, const FVector& Location)
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound, Location);
}
