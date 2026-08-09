// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/InteractionBehaviour/InteractionBehaviour.h"

UInteractionBehaviour::UInteractionBehaviour()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInteractionBehaviour::BeginPlay()
{
	Super::BeginPlay();
}