// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/PlayGameState.h"

#include "GameScene/PlayGameMode.h"
#include "GameScene/Player/MyPlayerState.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/CardSystem.h"
#include "GameScene/Player/Components/PlayerWalletSystem.h"
#include "Net/UnrealNetwork.h"


void APlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayGameState, RoundEndTime);
	DOREPLIFETIME(APlayGameState, RoundType);
}



void APlayGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	PlayerStateAdd.Broadcast(PlayerState);
}

void APlayGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	PlayerStateRemoved.Broadcast(PlayerState);
}


void APlayGameState::Server_GetPlayerKillEvent(AActor* Killer, AActor* Victim, bool bIsCriticalKill)
{
	AMyPlayerState* APS = nullptr;
	AMyPlayerState* BPS = nullptr;
	
	if(APlayerCharacter* A = Cast<APlayerCharacter>(Killer))
	{
		APS = A->GetPlayerState<AMyPlayerState>();
	}

	if(APlayerCharacter* B = Cast<APlayerCharacter>(Victim))
	{
		BPS = B->GetPlayerState<AMyPlayerState>();
	}

	if(IsValid(APS) && IsValid(BPS))
	{
		KillLogShowEvent.Broadcast(APS, BPS, bIsCriticalKill);
		NetMulticast_GetPlayerKillEvent(APS, BPS, bIsCriticalKill);
	}
}

void APlayGameState::NetMulticast_GetPlayerKillEvent_Implementation(APlayerState* KillerState, APlayerState* VictimState, bool bIsCritical)
{
	if(HasAuthority())
		return;

	KillLogShowEvent.Broadcast(KillerState, VictimState, bIsCritical);
}

void APlayGameState::SetRoundTimer(float ServerEndTime)
{
	RoundEndTime = ServerEndTime;

	RoundTimeChanged.Broadcast(RoundEndTime);
}

void APlayGameState::OnRep_RoundEndTime()
{
	RoundTimeChanged.Broadcast(RoundEndTime);
}

void APlayGameState::Multicast_SetRoundInfo_Implementation(int CurrentRound, FRoundInfo RoundInfo)
{
	if(HasAuthority())
		return;
	
	RoundInfoChanged.Broadcast(CurrentRound, RoundInfo);
}

void APlayGameState::SetRoundInfo(int CurrentRound, const FRoundInfo& RoundInfo)
{
	RoundType = RoundInfo.RoundType;
	RoundInfoChanged.Broadcast(CurrentRound, RoundInfo);
	
	Multicast_SetRoundInfo(CurrentRound, RoundInfo);
}

//server
const FRoundInfo* APlayGameState::GetCurrentRoundInfo()
{
	APlayGameMode* GM = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	if(!GM)
		return nullptr;

	return GM->GetRoundInfo();
}

//server
void APlayGameState::SetPlayerShotAvailable(bool bState)
{
	for(APlayerState* PS : PlayerArray)
	{
		APlayerCharacter* APC = Cast<APlayerCharacter>(PS->GetPawn());
		if(!APC)
		{
			continue;
		}

		APC->SetPlayerCanShot(bState);
	}
}

ERoundType APlayGameState::GetRoundType() const
{
	return RoundType;
}

void APlayGameState::GiveReward(const FRoundInfo& RoundInfo)
{
	if(RoundInfo.RecoveryLife > 0)
	{
		RecoveryAllPlayerLife(RoundInfo.RecoveryLife);
	}

	if(RoundInfo.RoundGold > 0)
	{
		GiveAllPlayerGold(RoundInfo.RoundGold);
	}

	if(RoundInfo.bGetCard)
	{
		GiveAllPlayerCard();
	}
}

void APlayGameState::RecoveryAllPlayerLife(int PlusLife)
{
	for(APlayerState* PS : PlayerArray)
	{
		if(AMyPlayerState* MyPS = Cast<AMyPlayerState>(PS))
		{
			MyPS->AddLife(PlusLife);
		}
	}
}

void APlayGameState::GiveAllPlayerGold(int PlusGold)
{
	for(APlayerState* PS : PlayerArray)
	{
		if(APlayerCharacter* APC = Cast<APlayerCharacter>(PS->GetPawn()))
		{
			APC->GetWalletSystem()->AcquireMoney(PlusGold);
		}
	}
}

void APlayGameState::GiveAllPlayerCard()
{
	//증강 시스템 만들때 카드 선택하게 만들기
	for(APlayerState* PS : PlayerArray)
	{
		if(APlayerCharacter* APC = Cast<APlayerCharacter>(PS->GetPawn()))
		{
			UE_LOG(LogTemp,Warning,TEXT("Request OpenCardMenu"));
			APC->GetCardSystem()->SelectCard();
		}
	}
}
