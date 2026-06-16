// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "CombatStructHeader.generated.h"


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
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.f, ClampMax=1.f))
	float CriticalDamageDefense;
	//방어무시
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







USTRUCT()
struct FPlayerDamageContext
{
	GENERATED_BODY()
	
	const FPlayerCombatStat* PlayerCombatStat;
	
	//payback 구독 델리게이트
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
	
	//플레이어가 때리는 경우 필요
	FPlayerDamageContext PlayerContext;
};

USTRUCT()
struct FDamageContextPayback
{
	GENERATED_BODY()
	
};
