// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogEntryUI.generated.h"

class UImage;
class UTextBlock;
class UKillLogEntryUI;

DECLARE_DELEGATE_OneParam(OnKillLogEnd, UKillLogEntryUI*);

UCLASS()
class RANDOMFPS_API UKillLogEntryUI : public UUserWidget
{
	GENERATED_BODY()

public:
	OnKillLogEnd KillLogEndEvent;

public:
	void StartShow();
	void StopShow();
	void SetKillLogData(APlayerState* Killer, APlayerState* Victim, bool bIsCritical);
	
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* KillerNameText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* VictimNameText;
	UPROPERTY(meta=(BindWidget))
	UImage* HeadShotImage;
	UPROPERTY(meta=(BindWidget))
	UImage* LogBackgroundImage;

	UPROPERTY(EditAnywhere)
	FLinearColor BlueColor;
	UPROPERTY(EditAnywhere)
	FLinearColor RedColor;
	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor;
	
	FTimerHandle ShowTimer;

private:
	void EndShow();
};
