// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyController.h"
#include "Lobby/LobbyUI.h"
#include "Blueprint/UserWidget.h"

void ALobbyController::BeginPlay()
{
	Super::BeginPlay();

	if(!IsLocalController()) return;
	
	LobbyUI = CreateWidget<ULobbyUI>(this, BP_LobbyUI);
	LobbyUI->AddToViewport();
}


void ALobbyController::OnRep_PlayerState()
{
	LobbyUI->RefreshLobbyGuests();
}
