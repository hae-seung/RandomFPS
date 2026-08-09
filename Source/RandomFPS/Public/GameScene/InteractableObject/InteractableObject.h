// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "InteractableObject.generated.h"

class UInteractionBehaviour;
class UInteractionUI;
class APlayerCharacter;
class UCameraComponent;
class UBoxComponent;
class UWidgetComponent;
class UPlayerInteractSystem;


UCLASS(Abstract)
class RANDOMFPS_API AInteractableObject :
public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractableObject();
	virtual void SetInteractState(bool bState) override;
	virtual void Interact(APlayerCharacter* APC) override;
	virtual void StopInteract(APlayerCharacter* APC) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UWidgetComponent* InteractInfoWidget; //f입력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UStaticMeshComponent* StaticMeshComp;

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	USceneComponent* DefaultRoot;
	UPROPERTY(EditAnywhere)
	FText InteractMsg;
	UPROPERTY()
	UInteractionUI* InteractionUI;//f입력
	
private:
	
};
