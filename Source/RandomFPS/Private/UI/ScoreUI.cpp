// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreUI.h"

#include "Components/VerticalBox.h"
#include "UI/ScoreEntryUI.h"
#include "GameScene/PlayGameState.h"
#include "GameScene/Player/MyPlayerState.h"

void UScoreUI::NativeOnInitialized()
{
	UE_LOG(LogTemp, Warning, TEXT("ScoreUI NativeInit"));
	
	if(APlayGameState* GS = GetWorld()->GetGameState<APlayGameState>())
	{
		GS->PlayerStateAdd.AddUObject(this, &UScoreUI::HandleAddNewPlayer);
		GS->PlayerStateRemoved.AddUObject(this, &UScoreUI::HandleRemovePlayer);
		
		for(auto& PS : GS->PlayerArray)
		{
			AddNewPlayerEntry(PS);
		}

		UpdateRank(0);
	}
}

void UScoreUI::HandleAddNewPlayer(APlayerState* PS)
{
	AddNewPlayerEntry(PS);
	UpdateRank(0);
}

void UScoreUI::HandleRemovePlayer(APlayerState* PS)
{
	RemovePlayerEntry(PS);
	UpdateRank(0);
}

void UScoreUI::AddNewPlayerEntry(APlayerState* PS)
{
	AMyPlayerState* MyPS = Cast<AMyPlayerState>(PS);
	
	if(PlayerStateMap.Contains(MyPS))
		return;

	UScoreEntryUI* NewEntry = CreateWidget<UScoreEntryUI>(this, WB_ScoreEntryUI);
	NewEntry->Init(MyPS);
	
	MyPS->LifeChanged.AddUObject(this, &UScoreUI::UpdateRank);
	
	RankSequence.Add(MyPS);
	PlayerStateMap.Add(MyPS, NewEntry);
	ScoreEntryList->AddChildToVerticalBox(NewEntry);
}

void UScoreUI::RemovePlayerEntry(APlayerState* PS)
{
	if(PlayerStateMap.Contains(PS))
	{
		UScoreEntryUI* Entry = PlayerStateMap.FindAndRemoveChecked(PS);
		ScoreEntryList->RemoveChild(Entry);
		RankSequence.RemoveSingle(Cast<AMyPlayerState>(PS));
	}
}

void UScoreUI::UpdateRank(int Life)
{
	ScoreEntryList->ClearChildren();

	RankSequence.Sort([](const AMyPlayerState& A, const AMyPlayerState& B) -> bool
	{
		if (A.GetLife() != B.GetLife())
		{
			return A.GetLife() > B.GetLife();
		}	

		if (A.GetKDARating() != B.GetKDARating())
		{
			return A.GetKDARating() > B.GetKDARating();
		}

		//닉네임은 오름차순
		return A.GetNickName() < B.GetNickName();
	});

	int CurrentRank = 1;
	for(int i = 0; i<RankSequence.Num(); ++i)
	{
		UScoreEntryUI* Entry = PlayerStateMap.FindRef(RankSequence[i]);
		ScoreEntryList->AddChildToVerticalBox(Entry);

		if(i > 0)
		{
			const AMyPlayerState* Prev = RankSequence[i-1];
			const AMyPlayerState* Cur = RankSequence[i];

			if(Prev->GetLife() != Cur->GetLife())
			{
				CurrentRank = i + 1;
			}
		}

		Entry->SetRankImage(RankImages[CurrentRank - 1]);
	}
}

void UScoreUI::FindLocalEntry(APlayerState* PS)
{
	if(!PlayerStateMap.Contains(PS))
	{
		AddNewPlayerEntry(PS);
	}

	UScoreEntryUI* ScoreEntryUI = PlayerStateMap.FindRef(PS);
	ScoreEntryUI->SetLocalImage();
}
