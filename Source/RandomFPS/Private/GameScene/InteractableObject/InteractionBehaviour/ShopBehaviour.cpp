// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/InteractionBehaviour/ShopBehaviour.h"

#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"


void UShopBehaviour::Interact(APlayerCharacter* APC)
{
	AMyPlayerController* AMC = APC->GetMyController();
	if(!IsValid(AMC))
		return;

	
}
