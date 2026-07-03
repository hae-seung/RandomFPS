// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "KillLogUI.generated.h"

class UOverlay;
class URichTextBlock;
class UVerticalBox;
class UKillLogEntryUI;

UCLASS()
class RANDOMFPS_API UKillLogUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(FOnKillAlarm& KillPlayerEvent, FOnAssistAlarm& KillAssistEvent);
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* KillLogList;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UKillLogEntryUI> WB_LogEntryUI;
	UPROPERTY()
	TArray<UKillLogEntryUI*> EntryPool;

	UPROPERTY(meta=(BindWidget))
	UOverlay* KillAlarmSet;
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* KillAlarmText;
	UPROPERTY()
	FTimerHandle AlarmTimer;

private:
	void ShowKillLog(APlayerState* Killer, APlayerState* Victim, bool bIsCritical);
	UKillLogEntryUI* GetEntryFromPool();
	void ReleasePool(UKillLogEntryUI* EntryUI);

	void StartTimer();
	void StopTimer();
	void HideAlarm();
	void SetKillAlarm(AActor* Victim);
	void SetKillAssist(AActor* Victim);
};
