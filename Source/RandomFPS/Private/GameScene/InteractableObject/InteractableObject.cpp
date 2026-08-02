// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/InteractableObject.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameScene/Player/PlayerCharacter.h"

AInteractableObject::AInteractableObject()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(DefaultRoot);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	InteractWidget->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComp->SetupAttachment(RootComponent);
}

void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	InteractCam = FindComponentByClass<UCameraComponent>();
}

void AInteractableObject::SetInteractState(bool bState)
{
	if(bState)
	{
		UE_LOG(LogTemp,Warning,TEXT("사용가능"));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("불가능"));
	}
}

void AInteractableObject::Interact(APlayerCharacter* APC)
{
	PlayerCharacter = APC;
}

void AInteractableObject::StopInteract()
{
	
}

UCameraComponent* AInteractableObject::GetCamera()
{
	return InteractCam;
}
