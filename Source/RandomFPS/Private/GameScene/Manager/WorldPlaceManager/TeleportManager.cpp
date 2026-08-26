// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Manager/WorldPlaceManager/TeleportManager.h"

#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerState.h"
#include "GameScene/PlayGameMode.h"
#include "GameScene/PlayGameState.h"

ATeleportManager::ATeleportManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
}

void ATeleportManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(!HasAuthority())
		return;
	
	UE_LOG(LogTemp,Warning, TEXT("PostInit Teleport"));

	if(APlayGameMode* GM = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->StartWaitRound.AddUObject(this, &ATeleportManager::TeleportRelaxRoom);
		GM->StartPlayRound.AddUObject(this, &ATeleportManager::TeleportRoundArea);
	}
	else
	{
		UE_LOG(LogTemp,Warning, TEXT("TeleportManger : No GM"));
	}
	
}

void ATeleportManager::BeginPlay()
{
	Super::BeginPlay();
}

void ATeleportManager::TeleportRelaxRoom()
{
	if(APlayGameState* GS = GetWorld()->GetGameState<APlayGameState>())
	{
		for(APlayerState* PS : GS->PlayerArray)
		{
			PS->GetPawn()->SetActorLocation(RelaxPoint->GetActorLocation());
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("TEleportManager : No GS"));
	}
}

void ATeleportManager::TeleportRoundArea(ERoundType RoundType)
{
	if(RoundType == ERoundType::Pve)
	{
		ShufflePoints(PvePoints);
		TeleportToArea(PvePoints);
	}
	else
	{
		ShufflePoints(PvpPoints);
		TeleportToArea(PvpPoints);
	}
}

void ATeleportManager::ShufflePoints(TArray<ATargetPoint*>& Points)
{
	for(int i = Points.Num() - 1; i > 0; i--)
	{
		const int j = FMath::RandRange(0, i);
		Points.Swap(i, j);
	}
}

void ATeleportManager::TeleportToArea(TArray<ATargetPoint*>& Points)
{
	if(APlayGameState* GS = GetWorld()->GetGameState<APlayGameState>())
	{
		int i = 0;
		for(APlayerState* PS : GS->PlayerArray)
		{
			PS->GetPawn()->SetActorLocation(Points[i]->GetActorLocation());
			i++;
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("TEleportManager : No GS"));
	}
}

