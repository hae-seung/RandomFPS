// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatStructHeader.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float MaxHP;
	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	UPROPERTY(EditAnywhere)
	float AttackDamage;
	UPROPERTY(EditAnywhere)
	float Defense;
	UPROPERTY(EditAnywhere)
	int MonsterFlinchProbability;
	UPROPERTY(EditAnywhere)
	float CriticalDamageDefense;
};

USTRUCT()
struct FPlayerDamageContext
{
	GENERATED_BODY()

	//데이터에셋에서 고정으로 생성되므로 포인터 접근 가능
	const FPlayerStat* PlayerStat;
	
	//payback 구독 델리게이트
};


USTRUCT()
struct FDamageContext
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Attacker;
	// UPROPERTY()
	// EEntityType EntityType;
	float Damage;
	bool bIsCritical;
	
	//플레이어가 때리는 경우 필요
	FPlayerDamageContext PlayerContext;
};

USTRUCT()
struct FDamageContextPayback
{
	GENERATED_BODY()
	
};
