// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/DamageActor.h"

#include "Components/WidgetComponent.h"
#include "UI/DamageUI.h"

ADamageActor::ADamageActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true); //위치는 전부 서버에서 결정할거임

	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	SetRootComponent(DamageWidgetComponent);
}


void ADamageActor::BeginPlay()
{
	Super::BeginPlay();

	DamageUI = Cast<UDamageUI>(DamageWidgetComponent->GetUserWidgetObject());
	if(DamageUI)
	{
		DamageUI->Init(DamageWidgetComponent);
	}
}


void ADamageActor::Acquire(AActor* NewOwner)
{
	bIsActive = true;

	SetActorHiddenInGame(false);
}

void ADamageActor::Release()
{
	bIsActive = false;

	SetActorHiddenInGame(true);
}

bool ADamageActor::IsActive() const
{
	return bIsActive;
}


void ADamageActor::SetDamageText(float DamageAmount, bool bIsCritical)
{
	DamageUI->SetDamage(DamageAmount, bIsCritical);
}