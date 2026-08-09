// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionBehaviour.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(InteractionBehaviour), meta=(BlueprintSpawnableComponent), Abstract)
class RANDOMFPS_API UInteractionBehaviour : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionBehaviour();
	virtual void Interact(APlayerCharacter* APC) PURE_VIRTUAL(UInteractionBehaviour::Interact,);

protected:
	virtual void BeginPlay() override;

};
