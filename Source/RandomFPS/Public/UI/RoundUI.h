// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/RoundStruct.h"
#include "RoundUI.generated.h"

class APlayGameState;
class USizeBox;
class UTextBlock;



UCLASS()
class RANDOMFPS_API URoundUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RoundText;
	UPROPERTY(meta=(BindWidget))
	USizeBox* DeathSet;
	UPROPERTY(meta=(BindWidget))
	USizeBox* CardSet;
	UPROPERTY(meta=(BindWidget))
	USizeBox* GoldSet;
	UPROPERTY(meta=(BindWidget))
	USizeBox* LifeHealthSet;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DeathText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* LifeHealthText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RemainTimeText;


private:
	UPROPERTY()
	APlayGameState* GS;

	float Server_RoundEndTime;
	float LastDisplayTime;
	
private:
	void ChangedRoundTime(float RoundEndTime);
	void ChangedRoundInfo(int Round, const FRoundInfo& RoundInfo);
};
