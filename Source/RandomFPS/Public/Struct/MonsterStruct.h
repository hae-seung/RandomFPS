// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterStruct.generated.h"


USTRUCT(BlueprintType)
struct FMonsterStat
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	EEntityType EntityType;
	UPROPERTY(EditAnywhere)
	float MaxHP;
	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	UPROPERTY(EditAnywhere)
	float AttackDamage;
	UPROPERTY(EditAnywhere)
	float K;
	
	//%단위로. ex) 17.5% => 0.175로 작성
	//공격 데미지 난수
	//최대 난수 +- 20%
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0.0f, ClampMax = 0.2f))
	float RandomAttackDamageRange;
	UPROPERTY(EditAnywhere)
	int CriticalProbability;
	UPROPERTY(EditAnywhere)
	float Defense;
	UPROPERTY(EditAnywhere)
	int Gold;
};


USTRUCT(BlueprintType)
struct FMonsterAttackDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere)
	float Radius;
	UPROPERTY(EditAnywhere)
	FName SocketName;
	UPROPERTY(EditAnywhere)
	float ForwardOffset;
	UPROPERTY(EditAnywhere)
	FVector LocalDirection;
	UPROPERTY(EditAnywhere)
	float Range;
	UPROPERTY(EditAnywhere)
	float DamageMultiplier = 1.0f;
};
