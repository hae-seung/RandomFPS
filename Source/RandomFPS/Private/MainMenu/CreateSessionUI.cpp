// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/CreateSessionUI.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "GameInstance/MyGameInstance.h"
#include "MainMenu/MainMenuUI.h"
#include "Misc/DefaultValueHelper.h"

void UCreateSessionUI::NativeOnInitialized()
{
	EditNameTxt->OnTextChanged.AddDynamic(this, &UCreateSessionUI::OnEditNameTxtChanged);
	EditNumTxt->OnTextChanged.AddDynamic(this, &UCreateSessionUI::OnEditNumTxtChanged);
	CreateBtn->OnClicked.AddDynamic(this, &UCreateSessionUI::OnCreateBtnClicked);
	CloseBtn->OnClicked.AddDynamic(this, &UCreateSessionUI::OnCloseBtnClicked);
}

void UCreateSessionUI::Init(UMainMenuUI* MainMenu)
{
	MainMenuUI = MainMenu;
}


void UCreateSessionUI::OnEditNameTxtChanged(const FText& Text)
{
	RoomName = Text.ToString();
}

void UCreateSessionUI::OnEditNumTxtChanged(const FText& Text)
{
	const FString Input = Text.ToString();

	int32 Value = 0;
	if(FDefaultValueHelper::ParseInt(Input, Value))
	{
		if(Value > LimitMaxPlayerCount || Value < 0)
		{
			EditNumTxt->SetText(FText::GetEmpty());
		}
		else
		{
			MaxPlayerCount = Value;
		}
	}
	else
	{
		EditNumTxt->SetText(FText::GetEmpty());
	}
}


void UCreateSessionUI::OnCreateBtnClicked()
{
	//GI에 요청.
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if(GI)
	{
		//성공이든 실패든 창 닫기
		OnCloseBtnClicked();

		//메인메뉴 버튼 활성 막기
		MainMenuUI->SetBtnInteract(false);
		
		GI->TryCreateSession(RoomName, MaxPlayerCount, bIsLanMode);
	}
	
}

void UCreateSessionUI::OnCloseBtnClicked()
{
	EditNameTxt->SetText(FText::GetEmpty());
	EditNumTxt->SetText(FText::GetEmpty());
	bIsLanMode = false;
	SetVisibility(ESlateVisibility::Collapsed);
}
