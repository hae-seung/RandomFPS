// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuUI.h"

#include "Components/Button.h"
#include "Components/Overlay.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainMenu/CreateSessionUI.h"
#include "MainMenu/SearchRoomUI.h"

void UMainMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	StartBtn->OnClicked.AddDynamic(this, &UMainMenuUI::OnClickedStartBtn);
	SettingBtn->OnClicked.AddDynamic(this,  &UMainMenuUI::OnClickedSettingBtn);
	ExitBtn->OnClicked.AddDynamic(this,  &UMainMenuUI::OnClickedExitBtn);

	HostBtn->OnClicked.AddDynamic(this, &UMainMenuUI::OnClickedHostBtn);
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenuUI::OnClickedJoinBtn);
	BackBtn->OnClicked.AddDynamic(this, &UMainMenuUI::OnClickedBackBtn);

	CreateSessionUI->Init(this);

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if(GI)
	{
		GI->OnCreateSessionFail.BindUObject(this, &UMainMenuUI::OnCreateSessionFail);
	}
	
}




void UMainMenuUI::OnClickedStartBtn()
{
	MenuBtnSet->SetVisibility(ESlateVisibility::Collapsed);
	PlayBtnSet->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuUI::OnClickedSettingBtn()
{
	
}

void UMainMenuUI::OnClickedExitBtn()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),
		nullptr,
		EQuitPreference::Quit,
		false);
}


//방만들기 버튼
void UMainMenuUI::OnClickedHostBtn()
{
	CreateSessionUI->SetVisibility(ESlateVisibility::Visible);
}

//방찾기 버튼
void UMainMenuUI::OnClickedJoinBtn()
{
	SearchRoomUI->Open();
}

void UMainMenuUI::OnClickedBackBtn()
{
	MenuBtnSet->SetVisibility(ESlateVisibility::Visible);
	PlayBtnSet->SetVisibility(ESlateVisibility::Collapsed);
}


void UMainMenuUI::OnCreateSessionFail()
{
	SetBtnInteract(true);
	
	//todo:방생성 실패팝업 UI 띄우기
	UE_LOG(LogTemp, Warning, TEXT("방생성 실패"));
}


void UMainMenuUI::SetBtnInteract(bool bInteract)const
{
	HostBtn->SetIsEnabled(bInteract);
	JoinBtn->SetIsEnabled(bInteract);
	BackBtn->SetIsEnabled(bInteract);
}


