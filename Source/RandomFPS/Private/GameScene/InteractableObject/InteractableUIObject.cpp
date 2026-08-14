// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/InteractableUIObject.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/WorldInteractionUI.h"

AInteractableUIObject::AInteractableUIObject()
{
	bReplicates = true;
	WorldInteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WorldInteractionUI"));
	WorldInteractionWidget->SetupAttachment(RootComponent);
	WorldInteractionWidget->SetWidgetSpace(EWidgetSpace::World);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(StaticMeshComp);
}

void AInteractableUIObject::BeginPlay()
{
	Super::BeginPlay();
	
	WorldInteractionUI =
		Cast<UWorldInteractionUI>(
			WorldInteractionWidget->GetUserWidgetObject());
}

void AInteractableUIObject::OpenUI(APlayerCharacter* APC)
{
	WorldInteractionUI->Open();
}