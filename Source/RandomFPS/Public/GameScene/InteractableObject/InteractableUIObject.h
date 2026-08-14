// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/InteractableObject/InteractableObject.h"
#include "InteractableUIObject.generated.h"


class UWorldInteractionUI;
class UInteractionBehaviour;

UCLASS(Abstract)
class RANDOMFPS_API AInteractableUIObject : public AInteractableObject
{
	GENERATED_BODY()

public:
	virtual void OpenUI(APlayerCharacter* APC);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UWidgetComponent* WorldInteractionWidget;
	UPROPERTY()
	UWorldInteractionUI* WorldInteractionUI;

	UPROPERTY(EditAnywhere)
	float CameraBlendTime;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
	
protected:
	AInteractableUIObject();
	virtual void BeginPlay() override;
};
