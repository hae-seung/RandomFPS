// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyUI.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WrapBox.h"
#include "GameInstance/MyGameInstance.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/Player/MyPlayerState.h"
#include "Lobby/LobbyGameMode.h"
#include "Lobby/LobbyGuestListItemUI.h"
#include "Lobby/LobbyGameState.h"

void ULobbyUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>())
	{
		//GameState는 서버든 클라든 무조건 먼저 생성되어있음.
		GS->OnPlayerListChangedEvent.BindUObject(this, &ULobbyUI::RefreshLobbyGuests);
		GS->OnReadyPlayerCntChangedEvent.BindUObject(this, &ULobbyUI::OnChangedRoomMembersReadyState);
	}
	

	SetBtns();
	
	RefreshLobbyGuests();
}


void ULobbyUI::RefreshLobbyGuests()
{
	//1.자기 자신을 제일 먼저 생성
	//2.자기 자신 제외 모두 생성

	MyPS = GetOwningPlayer()->GetPlayerState<AMyPlayerState>();
	if(!MyPS) return; //자기자신 PlayerState가 없다면 다른 PlayerState도 아직 Replicated 도착하지 못함.
	
	////////////////////////////////////////////////////////////
	if(!LobbyGuestItems.Contains(MyPS))
	{
		ULobbyGuestListItemUI* ClientInfo = CreateWidget<ULobbyGuestListItemUI>(this, BP_LobbyGuestItem);
		ClientInfo->Init(MyPS);
		LobbyGuestsBox->AddChild(ClientInfo);
		LobbyGuestItems.Add(MyPS, ClientInfo);
	}
	
	/////////////////////////////////////////////
	ALobbyGameState* GS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if(!GS) return;
	
	for(APlayerState* PS : GS->PlayerArray)
	{
		if(PS == MyPS) continue;

		if(!LobbyGuestItems.Contains(PS))
		{
			ULobbyGuestListItemUI* NewGuest = CreateWidget<ULobbyGuestListItemUI>(this, BP_LobbyGuestItem);
			NewGuest->Init(Cast<AMyPlayerState>(PS));
			LobbyGuestsBox->AddChild(NewGuest);
			LobbyGuestItems.Add(PS, NewGuest);
		}
	}
	//////////////////////////////////////////////

	
	//나간사람이 있는 경우
	TArray<TObjectPtr<APlayerState>> ToRemove;
	
	const TArray<TObjectPtr<APlayerState>>& Players = GS->PlayerArray;
	for(auto& Pair : LobbyGuestItems)
	{
		if(!Players.Contains(Pair.Key))
		{
			LobbyGuestsBox->RemoveChild(Pair.Value);
			ToRemove.Add(Pair.Key);
		}
	}

	for(TObjectPtr<APlayerState> Element : ToRemove)
	{
		LobbyGuestItems.Remove(Element);
	}
}

void ULobbyUI::OnClickNickNameChanged()
{
	if(NickNameTxt->GetText().IsEmptyOrWhitespace())
		return;

	if(!MyPS) return;

	MyPS->RequestSetNickName(NickNameTxt->GetText());
}


void ULobbyUI::SetBtns()
{
	NickNameChangeBtn->OnClicked.AddDynamic(this, &ULobbyUI::OnClickNickNameChanged);

	
	if(GetOwningPlayer()->HasAuthority())
	{
		ReadyBtn->SetVisibility(ESlateVisibility::Collapsed);
		StartBtn->SetVisibility(ESlateVisibility::Visible);
		StartBtn->SetIsEnabled(false);
	}
	else
	{
		ReadyBtn->SetVisibility(ESlateVisibility::Visible);
		StartBtn->SetVisibility(ESlateVisibility::Collapsed);
	}

	ReadyBtn->OnClicked.AddDynamic(this, &ULobbyUI::OnReadyBtnClicked);
	StartBtn->OnClicked.AddDynamic(this, &ULobbyUI::OnStartBtnClicked);
}


void ULobbyUI::OnReadyBtnClicked()
{
	AMyPlayerState* PS = GetOwningPlayer()->GetPlayerState<AMyPlayerState>();
	if(!PS) return;

	PS->Server_SetReadyState();
}

void ULobbyUI::OnStartBtnClicked()
{
	ALobbyGameMode* GM = GetWorld()->GetAuthGameMode<ALobbyGameMode>();
	if(GM)
	{
		UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
		if(GI)
		{
			FString URL = GI->GetWorldURL(EWorldName::MainMap);
			if(!URL.IsEmpty())
			{
				GetWorld()->ServerTravel(URL + "?listen");
				StartBtn->SetIsEnabled(false);
			}
		}
		
	}
}

void ULobbyUI::OnChangedRoomMembersReadyState(bool bIsAllReady)
{
	StartBtn->SetIsEnabled(bIsAllReady);
}
