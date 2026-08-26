// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoundUI.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameScene/PlayGameState.h"
#include "GameScene/Player/MyPlayerController.h"

void URoundUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!IsValid(GS) || Server_RoundEndTime <= 0)
		return;

	const float RemainTime = FMath::Max(
		0.f,
		Server_RoundEndTime - GS->GetServerWorldTimeSeconds());

	const int TotalSeconds = FMath::Floor(RemainTime);

	if(TotalSeconds == LastDisplayTime)
		return;

	LastDisplayTime = TotalSeconds;

	const int Minutes = TotalSeconds / 60;
	const int Seconds = TotalSeconds % 60;

	RemainTimeText->SetText(FText::Format(
		FText::FromString(TEXT("{0} : {1}")),
		Minutes, Seconds));
}

void URoundUI::Init()
{
	GS = Cast<APlayGameState>(GetWorld()->GetGameState());
	if(!GS)
	{
		UE_LOG(LogTemp,Warning, TEXT("NoGS"));
		return;
	}

	
	GS->RoundTimeChanged.AddUObject(this, &URoundUI::ChangedRoundTime);
	GS->RoundInfoChanged.AddUObject(this, &URoundUI::ChangedRoundInfo);
	

	//GameState에서는 Server_RPC는 불가능함
	//그래서 컨트롤러를 통해 경유
	if(AMyPlayerController* AMC = Cast<AMyPlayerController>(GetOwningPlayer()))
	{
		AMC->RequestPlayerReadyToStartRound();
	}
}

void URoundUI::ChangedRoundTime(float RoundEndTime)
{
	Server_RoundEndTime = RoundEndTime;
}

void URoundUI::ChangedRoundInfo(int32 Round, const FRoundInfo& RoundInfo)
{
	FText RoundTypeText;

	switch (RoundInfo.RoundType)
	{
		case ERoundType::Pve:
			RoundTypeText = FText::FromString(TEXT("Pve"));
			break;

		case ERoundType::Pvp:
			RoundTypeText = FText::FromString(TEXT("Pvp"));
			break;

		default:
			RoundTypeText = FText::FromString(TEXT("Unknown"));
			break;
	}

	RoundText->SetText(FText::Format(
		FText::FromString(TEXT("Round : {0}({1})")),
		Round,
		RoundTypeText
	));

	if(RoundInfo.RoundGold > 0)
	{
		GoldSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		GoldSet->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(RoundInfo.bGetCard)
	{
		CardSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		CardSet->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(RoundInfo.RecoveryLife > 0)
	{
		LifeHealthSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		LifeHealthText->SetText(FText::Format(
			FText::FromString(TEXT("+{0}")),
			RoundInfo.RecoveryLife));
	}
	else
	{
		LifeHealthSet->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(RoundInfo.RoundDeathLife > 0)
	{
		DeathSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		DeathText->SetText(FText::Format(
			FText::FromString(TEXT("-{0}")),
			RoundInfo.RoundDeathLife));
	}
	else
	{
		DeathSet->SetVisibility(ESlateVisibility::Collapsed);
	}
}
