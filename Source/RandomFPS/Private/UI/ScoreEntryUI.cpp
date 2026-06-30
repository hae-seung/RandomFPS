// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreEntryUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/MyPlayerState.h"


void UScoreEntryUI::Init(AMyPlayerState* PS)
{
	PS->KDAChanged.AddUObject(this, &UScoreEntryUI::UpdateKDAText);
	PS->MonsterKillChanged.AddUObject(this, &UScoreEntryUI::UpdateMonsterKillText);
	PS->LifeChanged.AddUObject(this, &UScoreEntryUI::UpdateLifeText);
	
	
	NameText->SetText(FText::FromString(PS->GetNickName()));
	UpdateKDAText(PS->GetKDA());
	UpdateMonsterKillText(PS->GetMonsterKill());
	UpdateLifeText(PS->GetLife());

	LocalBackgroundImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UScoreEntryUI::UpdateKDAText(const FPlayerKDA& KDA)
{
	KDAText->SetText(
	FText::Format(
		FText::FromString(TEXT("{0} / {1} / {2}")),
		KDA.Kill,
		KDA.Death,
		KDA.Assist));
}

void UScoreEntryUI::UpdateMonsterKillText(int MonsterKillCnt)
{
	MonsterKillText->SetText(FText::AsNumber(MonsterKillCnt));
}

void UScoreEntryUI::UpdateLifeText(int RemainLife)
{
	LifeText->SetText(FText::AsNumber(RemainLife));
}

void UScoreEntryUI::SetRankImage(UTexture2D* Image)
{
	RankImage->SetBrushFromTexture(Image);
}

void UScoreEntryUI::SetLocalImage()
{
	LocalBackgroundImage->SetVisibility(ESlateVisibility::HitTestInvisible);
}
