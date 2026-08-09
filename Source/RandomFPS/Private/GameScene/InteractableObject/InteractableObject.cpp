// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/InteractableObject.h"

#include "Components/WidgetComponent.h"
#include "GameScene/Player/Components/PlayerInteractSystem.h"
#include "UI/InteractionUI.h"

AInteractableObject::AInteractableObject()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(DefaultRoot);

	InteractInfoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	InteractInfoWidget->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComp->SetupAttachment(RootComponent);
}

void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	InteractionUI = Cast<UInteractionUI>(InteractInfoWidget->GetUserWidgetObject());
}

void AInteractableObject::SetInteractState(bool bState)
{
	if(bState)
	{
		InteractionUI->Show(InteractMsg);
	}
	else
	{
		InteractionUI->Hide();
	}
}

void AInteractableObject::Interact(APlayerCharacter* APC, UPlayerInteractSystem* InteractSystem)
{
	Server_APC = APC;
	Server_PlayerInteractSystem = InteractSystem;
}


void AInteractableObject::StopInteract()
{
	Server_PlayerInteractSystem->StopInteractMontage();
}
