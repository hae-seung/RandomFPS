// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillLogEntryUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/MyPlayerState.h"

void UKillLogEntryUI::StartShow()
{
	GetWorld()->GetTimerManager().SetTimer(
		ShowTimer,
		this, &UKillLogEntryUI::EndShow,
		3.f,
		false);

	SetVisibility(ESlateVisibility::Visible);
}

void UKillLogEntryUI::StopShow()
{
	GetWorld()->GetTimerManager().ClearTimer(ShowTimer);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UKillLogEntryUI::SetKillLogData(APlayerState* Killer, APlayerState* Victim, bool bIsCritical)
{
	if(APlayerState* LocalState = GetOwningPlayerState())
	{
		if(Killer == LocalState)
		{
			//blue
			LogBackgroundImage->SetColorAndOpacity(BlueColor);
		}
		else if(Victim == LocalState)
		{
			//red
			LogBackgroundImage->SetColorAndOpacity(RedColor);
		}
		else
		{
			//black
			LogBackgroundImage->SetColorAndOpacity(DefaultColor);
		}
	}

	if(bIsCritical)
	{
		HeadShotImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HeadShotImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	AMyPlayerState* A = Cast<AMyPlayerState>(Killer);
	KillerNameText->SetText(FText::FromString(A->GetNickName()));

	AMyPlayerState* B = Cast<AMyPlayerState>(Victim);
	VictimNameText->SetText(FText::FromString(B->GetNickName()));
}

void UKillLogEntryUI::EndShow()
{
	StopShow();
	KillLogEndEvent.ExecuteIfBound(this);
}

