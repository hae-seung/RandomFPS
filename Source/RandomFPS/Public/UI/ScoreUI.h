// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreUI.generated.h"

class AMyPlayerState;
class UScoreEntryUI;
class UVerticalBox;


UCLASS()
class RANDOMFPS_API UScoreUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void FindLocalEntry(APlayerState* PS);
	
protected:
	virtual void NativeOnInitialized() override;

private:
	//엔트리마다 top padding필요
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UVerticalBox* ScoreEntryList;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScoreEntryUI> WB_ScoreEntryUI;
	UPROPERTY(EditAnywhere)
	float PaddingAmount;
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> RankImages;

	//실제 등수별 UI순서
	//Life 따라서 변동
	UPROPERTY()
	TArray<AMyPlayerState*> RankSequence;
	UPROPERTY()
	TMap<APlayerState*, UScoreEntryUI*> PlayerStateMap;

private:
	void UpdateRank(int Life);

	void HandleAddNewPlayer(APlayerState* PS);
	void HandleRemovePlayer(APlayerState* PS);
	
	void AddNewPlayerEntry(APlayerState* PS);
	void RemovePlayerEntry(APlayerState* PS);
};
