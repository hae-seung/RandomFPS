// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "MonsterSpawner.generated.h"

class UPoolManager;
class ATargetPoint;
class AMonster;
class UMonsterData;



UCLASS()
class RANDOMFPS_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TArray<ATargetPoint*> SpawnPoints;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AMonster>> BP_Monsters;
	UPROPERTY(EditAnywhere)
	int FirstSpawnMonster;
	UPROPERTY(EditAnywhere)
	int TimerSpawnMonster;
	UPROPERTY(EditAnywhere)
	float SpawnBetTime;

	
	UPROPERTY()
	TArray<AMonster*> CurWorldSpawnMonsters;
	UPROPERTY()
	FTimerHandle SpawnTimer;
	UPROPERTY()
	UPoolManager* PoolManager;
	
	int PveRoundCount;
	int NewMonsterIndex;

private:
	void SpawnMonster(ERoundType RoundType);
	void StopSpawn();
	void SpawnNewMonster();
	void ShuffleSpawnPoints();
};
