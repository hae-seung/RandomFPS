// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "CombatStructHeader.generated.h"

#pragma region PlayerStat
USTRUCT(BlueprintType)
struct FPlayerHealthStat
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float MaxHp;
	UPROPERTY()
	float Hp;
};

USTRUCT(BlueprintType)
struct FPlayerCombatStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float AttackDamage;
	
	//방어력상수
	UPROPERTY(EditAnywhere)
	float K;
	UPROPERTY(EditAnywhere)
	float Defense;
	UPROPERTY(EditAnywhere)
	float DamageRand = 0.01f;
	UPROPERTY(EditAnywhere)
	float CriticalDamageBonus = 2.f;

	//치명타 데미지 감소
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.f, ClampMax=1.f))
	float CriticalDamageDefense;
	//방어율 감소
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.f, ClampMax=1.f))
	float DismissDefenseRate;

	//몬스터 기절확률
	UPROPERTY(EditAnywhere)
	int MonsterFlinchProbability;
};

USTRUCT()
struct FPlayerUtilityStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	UPROPERTY(EditAnywhere)
	int ReviveTime;
};
#pragma endregion



USTRUCT()
struct FPlayerDamageContext
{
	GENERATED_BODY()

	float FlinchProb;
	
};

//때릴때의 정보
USTRUCT()
struct FDamageContext
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Attacker;
	UPROPERTY()
	EEntityType EntityType;
	float BaseDamage;
	float FinalDamage;
	bool bIsCritical;

	FVector HitLocation;
	
	//플레이어가 때리는 경우 필요
	FPlayerCombatStat PlayerAttackerStat;
};

USTRUCT()
struct FPlayerKDA
{
	GENERATED_BODY()

	UPROPERTY()
	int Kill;
	UPROPERTY()
	int Death;
	UPROPERTY()
	int Assist;
};

