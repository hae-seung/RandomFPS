// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/Computer.h"

#include "Components/WidgetComponent.h"



AComputer::AComputer()
{
	PrimaryActorTick.bCanEverTick = false;

	ShopWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShopWidget"));
	ShopWidget->SetupAttachment(RootComponent);
}

void AComputer::BeginPlay()
{
	Super::BeginPlay();
	
}
