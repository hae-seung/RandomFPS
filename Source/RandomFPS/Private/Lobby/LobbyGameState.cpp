// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameState.h"

#include "Net/UnrealNetwork.h"


void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, CurrentRoomMemberCnt);
	DOREPLIFETIME(ALobbyGameState, ReadyPlayerCnt);
}




void ALobbyGameState::OnRep_PlayerArray()
{
	HandlePlayerListChanged();
}

void ALobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	CurrentRoomMemberCnt++;
	HandlePlayerListChanged();
}

void ALobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	CurrentRoomMemberCnt--;
	HandlePlayerListChanged();
}


void ALobbyGameState::HandlePlayerListChanged()
{
	OnPlayerListChangedEvent.ExecuteIfBound();
}



void ALobbyGameState::SetReadyState(bool bIsReady)
{
	if(!HasAuthority()) return;
	
	ReadyPlayerCnt = bIsReady ? ReadyPlayerCnt + 1 : ReadyPlayerCnt - 1;
	SetRoomMasterStartBtn();
}

void ALobbyGameState::OnRep_ReadyPlayerCnt()
{
	SetRoomMasterStartBtn();
}

void ALobbyGameState::SetRoomMasterStartBtn()
{
	if(ReadyPlayerCnt == CurrentRoomMemberCnt - 1) //방장제외 전부 레디인지
	{
		OnReadyPlayerCntChangedEvent.ExecuteIfBound(true);
	}
	else
	{
		OnReadyPlayerCntChangedEvent.ExecuteIfBound(false);
	}
}
