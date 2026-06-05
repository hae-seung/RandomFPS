// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/Parts.h"


AParts::AParts()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComponent);

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AParts::BeginPlay()
{
	Super::BeginPlay();
}
