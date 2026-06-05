// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameMode.h"

#include "GameScene/Player/MyPlayerState.h"
#include "Lobby/LobbyGameState.h"


ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}



void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	if(NewPlayer->IsLocalController())
	{
		AMyPlayerState* PS = NewPlayer->GetPlayerState<AMyPlayerState>();
		if(PS)
		{
			PS->SetRoomMaster();
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	
}

