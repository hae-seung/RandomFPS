// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/SearchRoomListItem.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USearchRoomListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CachedButtonStyle = SlotBtn->GetStyle();
}


void USearchRoomListItem::Init(OnClickedSlotBtn ClickEvent)
{
	ClickedSlotBtn = ClickEvent;
	SlotBtn->OnClicked.AddDynamic(this, &USearchRoomListItem::OnClickSlotBtn);
}

void USearchRoomListItem::SetRoomInfo(const FOnlineSessionSearchResult& Result)
{
	SessionSearchResult = Result;

	FString RoomName;
	Result.Session.SessionSettings.Get(
		FName("ROOM_NAME"),
		RoomName);

	const int MaxPlayer = Result.Session.SessionSettings.NumPublicConnections;
	const int CurrentPlayer = MaxPlayer - Result.Session.NumOpenPublicConnections;

	if(CurrentPlayer>=MaxPlayer)
	{
		SetButtonState(false, DisableColor);
	}
	else
	{
		SetButtonState(true, NormalBtnColor);
	}

	
	RoomNameTxt->SetText(FText::FromString(RoomName));
	PersonTxt->SetText(FText::FromString(FString::Printf(
		TEXT("%d / %d"), CurrentPlayer, MaxPlayer)));
}

void USearchRoomListItem::OnClickSlotBtn()
{
	SetButtonState(false, SelectedBtnColor);
	
	ClickedSlotBtn.Execute(this);
}

void USearchRoomListItem::SetButtonState(bool bEnable, FLinearColor ButtonColor)
{
	SlotBtn->SetIsEnabled(bEnable);
	CachedButtonStyle.Disabled.TintColor = ButtonColor;
	SlotBtn->SetStyle(CachedButtonStyle);
}


void USearchRoomListItem::SetButtonNormal()
{
	SetButtonState(true, NormalBtnColor);
}
