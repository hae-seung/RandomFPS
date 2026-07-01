// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillLogUI.h"

#include "Components/VerticalBox.h"
#include "GameScene/PlayGameState.h"
#include "UI/KillLogEntryUI.h"


void UKillLogUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

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
