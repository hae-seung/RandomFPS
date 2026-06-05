// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/CombatStructHeader.h"
#include "Struct/MonsterStruct.h"
#include "MonsterCombatSystem.generated.h"

class UMonsterData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UMonsterCombatSystem : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UMonsterCombatSystem();
	void Init(UMonsterData* Data);
	void ApplyDamage(FHitResult& HitResult);

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY()
	FMonsterStat Stat;
	

private:
	float CalculateAttackDamage(FDamageContext& AttackContext);
};
