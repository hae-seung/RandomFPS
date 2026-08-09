// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/InteractableObject/InteractionBehaviour/InteractionBehaviour.h"
#include "ShopBehaviour.generated.h"


class APlayerCharacter;

UCLASS(ClassGroup=(InteractionBehaviour))
class RANDOMFPS_API UShopBehaviour : public UInteractionBehaviour
{
	GENERATED_BODY()

	
public:
	virtual void Interact(APlayerCharacter* APC) override;
};
