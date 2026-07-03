// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillLogUI.h"

#include "Components/Overlay.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "GameScene/PlayGameState.h"
#include "GameScene/Player/MyPlayerState.h"
#include "UI/KillLogEntryUI.h"

void UKillLogUI::Init(FOnKillAlarm& KillPlayerEvent, FOnAssistAlarm& KillAssistEvent)
{
	KillPlayerEvent.AddUObject(this, &UKillLogUI::SetKillAlarm);
	KillAssistEvent.AddUObject(this, &UKillLogUI::SetKillAssist);
}


void UKillLogUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	KillAlarmSet->SetVisibility(ESlateVisibility::Collapsed);
	
	APlayGameState* PlayGS = GetWorld()->GetGameState<APlayGameState>();
	if(PlayGS)
	{
		PlayGS->KillLogShowEvent.AddUObject(this, &UKillLogUI::ShowKillLog);
	}
	
}

void UKillLogUI::ShowKillLog(APlayerState* Killer, APlayerState* Victim, bool bIsCritical)
{
	UKillLogEntryUI* ShowEntry = nullptr;
	
	if(KillLogList->GetChildrenCount() >= 6)
	{
		ShowEntry = Cast<UKillLogEntryUI>(
			KillLogList->GetChildAt(
				KillLogList->GetChildrenCount() - 1));

		ShowEntry->StopShow();
		ReleasePool(ShowEntry);
	}

	if(ShowEntry == nullptr)
	{
		ShowEntry = GetEntryFromPool();
	}

	if(KillLogList->GetChildrenCount() == 0)
	{
		KillLogList->AddChildToVerticalBox(ShowEntry);
	}
	else
	{
		KillLogList->InsertChildAt(0, ShowEntry);
	}
	
	ShowEntry->SetKillLogData(Killer, Victim, bIsCritical);
	ShowEntry->StartShow();
}

UKillLogEntryUI* UKillLogUI::GetEntryFromPool()
{
	UKillLogEntryUI* NewEntry = nullptr;
	if(EntryPool.Num() == 0)
	{
		NewEntry = CreateWidget<UKillLogEntryUI>(this, WB_LogEntryUI);
		NewEntry->KillLogEndEvent.BindUObject(this, &UKillLogUI::ReleasePool);
	}
	else
	{
		NewEntry = EntryPool.Last();
		EntryPool.Pop(EAllowShrinking::No);
	}

	return NewEntry;
}

void UKillLogUI::ReleasePool(UKillLogEntryUI* EntryUI)
{
	KillLogList->RemoveChild(EntryUI);
	EntryPool.Add(EntryUI);
}

void UKillLogUI::SetKillAlarm(AActor* Victim)
{
	KillAlarmSet->SetVisibility(ESlateVisibility::Collapsed);
	StopTimer();

	APlayerCharacter* B = Cast<APlayerCharacter>(Victim);
	AMyPlayerState* MyB = Cast<AMyPlayerState>(B->GetPlayerState());
	
	FText Text = FText::Format(
		FText::FromString(TEXT("당신이 <Name>{0}</>님을 <What>처치</>하였습니다")),
		FText::FromString(MyB->GetNickName()));
	KillAlarmText->SetText(Text);

	KillAlarmSet->SetVisibility(ESlateVisibility::Visible);
	
	StartTimer();
}

void UKillLogUI::SetKillAssist(AActor* Victim)
{
	StopTimer();
	APlayerCharacter* B = Cast<APlayerCharacter>(Victim);
	AMyPlayerState* MyB = Cast<AMyPlayerState>(B->GetPlayerState());

	FText Text = FText::Format(
		FText::FromString(TEXT("당신이 <Name>{0}</>님 처지에 <What>도움</>을 주었습니다")),
		FText::FromString(MyB->GetNickName()));

	KillAlarmText->SetText(Text);
	KillAlarmSet->SetVisibility(ESlateVisibility::Visible);
	StartTimer();
}


void UKillLogUI::HideAlarm()
{
	KillAlarmSet->SetVisibility(ESlateVisibility::Collapsed);
}

void UKillLogUI::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		AlarmTimer,
		this, &UKillLogUI::HideAlarm,
		3.f,
		false);
}

void UKillLogUI::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AlarmTimer);
}


