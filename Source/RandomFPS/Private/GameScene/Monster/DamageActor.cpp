// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/DamageActor.h"

#include "Components/WidgetComponent.h"
#include "GameScene/PoolManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/DamageUI.h"

ADamageActor::ADamageActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
	AActor::SetReplicateMovement(false);

	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	SetRootComponent(DamageWidgetComponent);
}

void ADamageActor::Tick(float DeltaSeconds)
{
	RotateToLocalCamera();
}


void ADamageActor::BeginPlay()
{
	Super::BeginPlay();

	DamageUI = Cast<UDamageUI>(DamageWidgetComponent->GetUserWidgetObject());
	PC = GetWorld()->GetFirstPlayerController();
}


void ADamageActor::Acquire(AActor* NewOwner)
{
	bIsActive = true;

	SetOwner(NewOwner);
	SetInstigator(Cast<APawn>(NewOwner));
	
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);

	StartReleaseTimer();
}

void ADamageActor::Release()
{
	bIsActive = false;
	SetActorTickEnabled(false);

	SetOwner(nullptr);
	SetInstigator(nullptr);
	
	SetActorHiddenInGame(true);

	UPoolManager* PoolManager = GetWorld()->GetSubsystem<UPoolManager>();
	PoolManager->Server_ReleaseActor(this);
}

void ADamageActor::StartReleaseTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this, &ADamageActor::Release,
		LifeTime,
		false);
}


bool ADamageActor::IsActive() const
{
	return bIsActive;
}

void ADamageActor::SetDamageText(float DamageAmount, bool bIsCritical)
{
	DamageUI->SetDamage(DamageAmount, bIsCritical);

	RotateToLocalCamera();
	
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this, &ADamageActor::Release,
		LifeTime,
		false);
}

void ADamageActor::RotateToLocalCamera()
{
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	LookAtRot = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		CameraLocation);

	SetActorRotation(LookAtRot);
}
