// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/PlayGameMode.h"

#include "GameInstance/MyGameInstance.h"
#include "GameScene/PlayGameState.h"

APlayGameMode::APlayGameMode()
{
	bUseSeamlessTravel = true;
}

void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		const int PlayerNum = GI->GetTotalPlayers();
		if(PlayerNum > 0)
			ExpectedPlayerNum = GI->GetTotalPlayers();

		bInitPlayerNum = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("GameMode 시작! 기대 인원(ExpectedPlayerNum): %d"), ExpectedPlayerNum);
	CheckToPlayStart();
}

void APlayGameMode::AddFinishInitPlayer()
{
	FinishInitPlayerNum++;
}


void APlayGameMode::CheckToPlayStart()
{
	if(!bInitPlayerNum)
		return;
	
	if(ExpectedPlayerNum == 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("ExpectedPlayerNum is 0"));
		return;
	}

	if(ExpectedPlayerNum == FinishInitPlayerNum)
	{
		//라운드 시작
		if(!bStartGame)
			StartRelaxRound();
	}
}

void APlayGameMode::StartRelaxRound()
{
	UE_LOG(LogTemp,Warning, TEXT("쉬는시간 시작!"));
	bStartGame = true;
	
	//쉬는 구역으로 텔레포트
	StartWaitRound.Broadcast();
	
	
	//쉬는시간 지급 및 아이템 지급
	float EndRelaxTime = GetWorld()->GetTimeSeconds() + RelaxTime;
	APlayGameState* GS = GetGameState<APlayGameState>();
	if(!GS)
	{
		UE_LOG(LogTemp,Warning, TEXT("RelaxTime No GS"));
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		RelaxTimer,
		this, &APlayGameMode::EndRelaxTime,
		RelaxTime,
		false);

	GS->SetRoundTimer(EndRelaxTime);
	GS->SetRoundInfo(Round, RoundInfos[Round]); //UI
	GS->SetPlayerShotAvailable(false);
	

	//아이템 지급은 GameState에서
	GS->GiveReward(RoundInfos[Round]);
}

void APlayGameMode::EndRelaxTime()
{
	UE_LOG(LogTemp,Warning, TEXT("쉬는 시간 종료"));
	StartRound();
}

void APlayGameMode::StartRound()
{
	UE_LOG(LogTemp,Warning, TEXT("Round 실제 시작!"));
	
	//pve, pvp 구역으로 텔레포트
	StartPlayRound.Broadcast(RoundInfos[Round].RoundType);

	const float RoundDuration = (Round & 1) == 0 ? PvpTime : PveTime;

	const float RoundEndTime = GetWorld()->GetTimeSeconds() + RoundDuration;
	
	GetWorld()->GetTimerManager().SetTimer(
		RoundTimer,
		this, &APlayGameMode::EndRoundTime,
		RoundDuration,
		false);

	APlayGameState* GS = GetGameState<APlayGameState>();
	if(!GS)
	{
		UE_LOG(LogTemp,Warning, TEXT("RelaxTime No GS"));
		return;
	}

	GS->SetRoundTimer(RoundEndTime);
	GS->SetPlayerShotAvailable(true);
}

void APlayGameMode::EndRoundTime()
{
	UE_LOG(LogTemp,Warning,TEXT("라운드 종료"));
	Round++;
	if(Round >= RoundInfos.Num())
	{
		EndGamePlay();
		return;
	}
	
	StartRelaxRound();
}

void APlayGameMode::EndGamePlay()
{
	UE_LOG(LogTemp,Warning,TEXT("게임종료"));
}

const FRoundInfo* APlayGameMode::GetRoundInfo()
{
	return &RoundInfos[Round];
}