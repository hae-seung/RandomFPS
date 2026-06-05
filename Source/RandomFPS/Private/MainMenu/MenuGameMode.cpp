// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MenuGameMode.h"

#include "GameFramework/PlayerState.h"

AMenuGameMode::AMenuGameMode()
{
	DefaultPawnClass = nullptr;
	
	HUDClass = nullptr;
	GameStateClass = nullptr;
	PlayerStateClass = APlayerState::StaticClass();
}
