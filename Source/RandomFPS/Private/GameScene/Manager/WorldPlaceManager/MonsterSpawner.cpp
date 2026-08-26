// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Manager/WorldPlaceManager/MonsterSpawner.h"

#include "Engine/TargetPoint.h"
#include "GameScene/PlayGameMode.h"
#include "GameScene/PoolManager.h"
#include "GameScene/Monster/Monster.h"


AMonsterSpawner::AMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
}

void AMonsterSpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(!HasAuthority())
		return;

	if(APlayGameMode* GM = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->StartPlayRound.AddUObject(this, &AMonsterSpawner::SpawnMonster);
		GM->StartWaitRound.AddUObject(this, &AMonsterSpawner::StopSpawn);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawner : No GM"));
	}
}

void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	PoolManager = GetWorld()->GetSubsystem<UPoolManager>();
}

void AMonsterSpawner::SpawnMonster(ERoundType RoundType)
{
	if(RoundType == ERoundType::Pvp)
		return;

	PveRoundCount++;

	//30초마다 6마리씩 소환
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimer,
		this, &AMonsterSpawner::SpawnNewMonster,
		SpawnBetTime,
		true);

	
	NewMonsterIndex = FMath::RandRange(0, BP_Monsters.Num() - 1);

	ShuffleSpawnPoints();
	for(int i = 0; i < FirstSpawnMonster; i++)
	{
		AMonster* NewMonster = Cast<AMonster>(
		PoolManager->Server_GetActor(BP_Monsters[NewMonsterIndex], this));

		if(NewMonster)
		{
			NewMonster->SetMonsterReinforceData(PveRoundCount);
			CurWorldSpawnMonsters.Add(NewMonster);
			NewMonster->SetActorLocation(SpawnPoints[i]->GetActorLocation());
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("No Monster From Pool"));
		}
	}
}

void AMonsterSpawner::StopSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);

	for(int i = 0; i < CurWorldSpawnMonsters.Num(); i++)
	{
		//실제 몬스터는 죽었지만 죽는 애니메이션(3초)로 인해
		//풀에는 아직 반환 안된 상태일 수 있어서 중복으로 반환을 막기위함.
		//dead는 애니메이션 상관없이 죽는 그 즉시 세팅되는 값임.
		if(!CurWorldSpawnMonsters[i]->GetIsDead())
			CurWorldSpawnMonsters[i]->NotDeadButRequestReleasePool(true);
	}

	CurWorldSpawnMonsters.Reset();
}

void AMonsterSpawner::SpawnNewMonster()
{
	ShuffleSpawnPoints();
	
	for(int i = 0; i < TimerSpawnMonster; i++)
	{
		AMonster* NewMonster = Cast<AMonster>(
		PoolManager->Server_GetActor(BP_Monsters[NewMonsterIndex], this));

		if(NewMonster)
		{
			NewMonster->SetMonsterReinforceData(PveRoundCount);
			CurWorldSpawnMonsters.Add(NewMonster);
			NewMonster->SetActorLocation(SpawnPoints[i]->GetActorLocation());
		}
	}
}

void AMonsterSpawner::ShuffleSpawnPoints()
{
	for(int i = SpawnPoints.Num() - 1; i > 0; i--)
	{
		const int j = FMath::RandRange(0, i);
		SpawnPoints.Swap(i, j);
	}
}
