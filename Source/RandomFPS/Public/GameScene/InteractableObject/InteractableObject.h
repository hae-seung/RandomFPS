// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

class APlayerCharacter;
class UCameraComponent;
class UBoxComponent;
class UWidgetComponent;

UCLASS(Abstract)
class RANDOMFPS_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableObject();
	void SetInteractState(bool bState);
	void Interact(APlayerCharacter* APC);
	void StopInteract();
	UCameraComponent* GetCamera();
	
protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	USceneComponent* DefaultRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UWidgetComponent* InteractWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UStaticMeshComponent* StaticMeshComp;

private:
	UPROPERTY()
	UCameraComponent* InteractCam;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	
private:
	
};
