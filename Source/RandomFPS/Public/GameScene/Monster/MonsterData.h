// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameScene/Cards/StructHeader.h"
#include "Struct/MonsterStruct.h"

#include "MonsterData.generated.h"


class ADamageActor;

UCLASS()
class RANDOMFPS_API UMonsterData : public UDataAsset
{
	GENERATED_BODY()


public:
	FORCEINLINE EEntityType GetEntityType() const { return EntityType; }
	FORCEINLINE bool IsUseSight() const{ return bUseSight; }
	FORCEINLINE const FAISightPerceptionInfo& GetSightInfo() const{ return SightPerceptionInfo; }
	FORCEINLINE const FMonsterAttackDefinition& GetAttackDefinition() const { return AttackDefinition; }
	FORCEINLINE const FMonsterStat& GetMonsterStatData() const { return MonsterStat; }
	FORCEINLINE float GetAttackRange() const { return AttackRangeRadius; }
	FORCEINLINE UAnimMontage* GetFlinchMontage() const { return FlinchMontage; }
	FORCEINLINE UAnimMontage* GetDeathMontage() const { return DeathMontage; }
	FORCEINLINE UNiagaraSystem* GetBloodEffect() const { return HitBlood; }
	FORCEINLINE TSubclassOf<ADamageActor> GetBP_DamageActor() const { return DamageActor; }
	
private:
	UPROPERTY(EditAnywhere)
	EEntityType EntityType;
	
	//시야 감각
	UPROPERTY(EditAnywhere)
	bool bUseSight;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "bUseSight", EditConditionHides))
	FAISightPerceptionInfo SightPerceptionInfo;

	UPROPERTY(EditAnywhere)
	FMonsterStat MonsterStat;
	UPROPERTY(EditAnywhere)
	FMonsterAttackDefinition AttackDefinition;
	UPROPERTY(EditAnywhere)
	float AttackRangeRadius;

	UPROPERTY(EditAnywhere)
	UAnimMontage* FlinchMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HitBlood;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADamageActor> DamageActor;
};
