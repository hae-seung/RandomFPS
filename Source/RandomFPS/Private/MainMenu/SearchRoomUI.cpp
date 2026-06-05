// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/SearchRoomUI.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/TileView.h"
#include "Components/WrapBox.h"
#include "GameInstance/MyGameInstance.h"
#include "MainMenu/SearchRoomListItem.h"

void USearchRoomUI::NativeOnInitialized()
{
	CloseBtn->OnClicked.AddDynamic(this, &USearchRoomUI::OnClickedCloseBtn);
	JoinBtn->OnClicked.AddDynamic(this, &USearchRoomUI::OnClickJoinBtn);
	
	//GI Search 구독
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if(GI)
	{
		GI->OnFindSteamSessionComplete.BindUObject(this, &USearchRoomUI::OnSearchSteamRoomComplete);
		GI->OnJoinSessionFail.BindUObject(this, &USearchRoomUI::OnJoinSessionFail);
		GI->OnJoinSessionSuccess.BindUObject(this, &USearchRoomUI::OnJoinSessionSuccess);
	}

	
}

void USearchRoomUI::Open()
{
	SetVisibility(ESlateVisibility::Visible);
	Init();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if(GI)
	{
		GI->FindSession(true);
	}
}


void USearchRoomUI::Init()
{
	CurrentSelectRoom = nullptr;
	
	for(USearchRoomListItem* Room : SteamRooms)
	{
		Room->SetVisibility(ESlateVisibility::Collapsed);
	}
}



void USearchRoomUI::OnClickedCloseBtn()
{
	SetVisibility(ESlateVisibility::Collapsed);
}



void USearchRoomUI::OnSearchSteamRoomComplete(const TArray<FOnlineSessionSearchResult>& Rooms)
{
	//Steam 방 띄우기
	int32 RoomIndex = 0;
	for(; RoomIndex<Rooms.Num(); RoomIndex++)
	{
		//방UI부족 => 생성
		if(RoomIndex >= SteamRooms.Num())
		{
			USearchRoomListItem* NewRoomSlot = CreateWidget<USearchRoomListItem>(this, BP_Room);
			NewRoomSlot->Init(OnClickedSlotBtn::CreateUObject(
					this, &USearchRoomUI::SetCurrentSelectRoom));//버튼 등록
			SteamListView->AddChild(NewRoomSlot);
			SteamRooms.Add(NewRoomSlot);
		}

		SteamRooms[RoomIndex]->SetRoomInfo(Rooms[RoomIndex]);
		SteamRooms[RoomIndex]->SetVisibility(ESlateVisibility::Visible);
	}

	//남는슬롯 비활성화
	for(int i = RoomIndex; i<SteamRooms.Num(); i++)
	{
		SteamRooms[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void USearchRoomUI::SetCurrentSelectRoom(USearchRoomListItem* Room)
{
	if(CurrentSelectRoom != nullptr)
		CurrentSelectRoom->SetButtonNormal();
	
	CurrentSelectRoom = Room;
}


void USearchRoomUI::OnJoinSessionSuccess()
{
	
}


void USearchRoomUI::OnJoinSessionFail()
{
	SetVisibility(ESlateVisibility::Collapsed);
}



void USearchRoomUI::OnClickJoinBtn()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if(GI && CurrentSelectRoom != nullptr)
	{
		GI->TryJoinSession(CurrentSelectRoom->GetSessionSearchResult());
	}
}
