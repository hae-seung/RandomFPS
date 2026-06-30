// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/CombatStructHeader.h"
#include "ScoreEntryUI.generated.h"

class AMyPlayerState;
class UTextBlock;
class UImage;


UCLASS()
class RANDOMFPS_API UScoreEntryUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(AMyPlayerState* PS);
	void SetRankImage(UTexture2D* Image);
	void SetLocalImage();

private:
	UPROPERTY(meta=(BindWidget))
	UImage* RankImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta=(BindWidget))
	UImage* LocalBackgroundImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* KDAText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MonsterKillText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* LifeText;
	


private:
	void UpdateKDAText(const FPlayerKDA& KDA);
	void UpdateMonsterKillText(int MonsterKillCnt);
	void UpdateLifeText(int RemainLife);
};
