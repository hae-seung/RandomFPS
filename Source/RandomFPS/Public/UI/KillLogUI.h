// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogUI.generated.h"

class UVerticalBox;
class UKillLogEntryUI;

UCLASS()
class RANDOMFPS_API UKillLogUI : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* KillLogList;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UKillLogEntryUI> WB_LogEntryUI;

	
	TArray<UKillLogEntryUI*> EntryPool;

private:
	void ShowKillLog(APlayerState* Killer, APlayerState* Victim, bool bIsCritical);
	UKillLogEntryUI* GetEntryFromPool();
	void ReleasePool(UKillLogEntryUI* EntryUI);
};
