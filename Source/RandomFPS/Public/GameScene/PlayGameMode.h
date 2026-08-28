// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Struct/RoundStruct.h"
#include "PlayGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(OnStartRelaxRound);
DECLARE_MULTICAST_DELEGATE_OneParam(OnStartPlayRound, ERoundType RoundType);
DECLARE_MULTICAST_DELEGATE(On)

UCLASS()
class RANDOMFPS_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()


public:
	//텔레포트용
	OnStartRelaxRound StartWaitRound;
	OnStartPlayRound StartPlayRound;

	
public:
	void AddFinishInitPlayer();
	void CheckToPlayStart();
	const FRoundInfo* GetRoundInfo();
	
protected:
	APlayGameMode();
	virtual void BeginPlay() override;
	
private:
	//증강은 2 5 8 11라운드에서 획득
	UPROPERTY(EditAnywhere)
	TArray<FRoundInfo> RoundInfos;
	UPROPERTY(EditAnywhere)
	float RelaxTime;
	UPROPERTY(EditAnywhere)
	float PveTime;
	UPROPERTY(EditAnywhere)
	float PvpTime;

	//임시
	UPROPERTY(EditAnywhere)
	int ExpectedPlayerNum;
	
private:
	bool bStartGame = false;
	int Round = 1;
	int FinishInitPlayerNum = 0;

	UPROPERTY()
	FTimerHandle RelaxTimer;
	UPROPERTY()
	FTimerHandle RoundTimer;

private:
	void StartRelaxRound();
	void EndRelaxTime();
	void StartRound();
	void EndRoundTime();
	void EndGamePlay();
};
