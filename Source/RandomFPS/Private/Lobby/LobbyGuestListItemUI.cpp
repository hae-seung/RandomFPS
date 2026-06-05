// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGuestListItemUI.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/MyPlayerState.h"

void ULobbyGuestListItemUI::Init(AMyPlayerState* PS)
{
	CachedPlayerState = PS;
	
	//구독
	PS->NickNameChangedEvent.AddUObject(this, &ULobbyGuestListItemUI::OnNickNameChangedReceive);
	PS->ReadyStateChangedEvent.AddUObject(this, &ULobbyGuestListItemUI::OnReadyStateChangedReceive);
	
	
	LobbyMasterImage->SetVisibility(ESlateVisibility::Collapsed);
	ReadySet->SetVisibility(ESlateVisibility::Hidden);
	
	PlayerNameTxt->SetText(FText::FromString(PS->GetNickName()));
	CheckIsPlayerMaster();
}


void ULobbyGuestListItemUI::Refresh()
{
	LobbyMasterImage->SetVisibility(ESlateVisibility::Collapsed);
	ReadySet->SetVisibility(ESlateVisibility::Hidden);
	
	PlayerNameTxt->SetText(FText::FromString(CachedPlayerState->GetNickName()));
	CheckIsPlayerMaster();
}


void ULobbyGuestListItemUI::CheckIsPlayerMaster()
{
	if(CachedPlayerState->CheckIsRoomMaster())
	{
		LobbyMasterImage->SetVisibility(ESlateVisibility::Visible);
	}
}



void ULobbyGuestListItemUI::OnNickNameChangedReceive()
{
	PlayerNameTxt->SetText(FText::FromString(CachedPlayerState->GetNickName()));
}

void ULobbyGuestListItemUI::OnReadyStateChangedReceive()
{
	if(CachedPlayerState->GetIsReady())
	{
		ReadySet->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReadySet->SetVisibility(ESlateVisibility::Hidden);
	}
}
