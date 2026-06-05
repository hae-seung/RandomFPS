// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterAttackSystem.generated.h"


struct FMonsterAttackDefinition;
class AMonster;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UMonsterAttackSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMonsterAttackSystem();
	void StartAttack();
	void EndAttack();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AMonster* Monster;
	
	const FMonsterAttackDefinition* Def;
	
	FVector AttackStartLocation;
	FVector AttackEndLocation;

	FCollisionShape Shape;
	
private:
	//근접공격
	void TraceAttack();
};
