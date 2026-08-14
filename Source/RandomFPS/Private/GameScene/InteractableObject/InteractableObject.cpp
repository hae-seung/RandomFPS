// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/InteractableObject.h"

#include "Components/WidgetComponent.h"
#include "GameScene/Player/PlayerCharacter.h"
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
	InteractInfoWidget->SetWidgetSpace(EWidgetSpace::Screen);

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

void AInteractableObject::Interact(APlayerCharacter* APC)
{
	//empty space
}


void AInteractableObject::StopInteract(APlayerCharacter* APC)
{
	//상호작용을 하는게 단발성 이벤트가 아닌 지속성인 경우에 실행되어야 하는 함수
	UPlayerInteractSystem* InteractSystem = APC->GetInteractSystem();
	InteractSystem->StopInteractMontage();
	APC->ToggleCharacterMoveState(true);
}
