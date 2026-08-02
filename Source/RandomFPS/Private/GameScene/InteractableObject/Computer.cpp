// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/Computer.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"



AComputer::AComputer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AComputer::BeginPlay()
{
	Super::BeginPlay();
	
}
