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
	//PlayerState로부터 GetPawn을 하면 안됨.
	//PlayerState에 아직 Pawn이 안들어왓을 수 있음
	//GM에 플레이어 로딩완료 상태는 ClientUI가 완성되어야 옴
	//이때 ClientUI는 컨트롤러에서 OnRep_Pawn일때 만들어짐
	//즉 서버에서는 이미 Pawn과 컨트롤러가 연결 끝났음을 의미해서 이게 가장 확실함
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (APawn* Pawn = PC->GetPawn())
			{	// Possess된 실체 Pawn에 바로 접근
				Pawn->TeleportTo(RelaxPoint->GetActorLocation(), RelaxPoint->GetActorRotation());
			}
		}
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
			PS->GetPawn()->TeleportTo(Points[i]->GetActorLocation(), Points[i]->GetActorRotation());
			i++;
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("TEleportManager : No GS"));
	}
}

