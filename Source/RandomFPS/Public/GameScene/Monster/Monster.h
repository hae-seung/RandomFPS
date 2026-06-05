// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Damageable.h"
#include "Struct/MonsterStruct.h"
#include "Monster.generated.h"

class UMonsterAttackSystem;
class AMonsterController;
class UMonsterCombatSystem;
class UMonsterData;
class UBehaviorTree;

UCLASS()
class RANDOMFPS_API AMonster : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AMonster();
	UBehaviorTree* GetBT() const;
	FORCEINLINE UMonsterData* GetData() const { return Data; }
	FORCEINLINE const FMonsterAttackDefinition* GetCurAttackDefinition() const { return CurrentAttackDefinition; }
	FORCEINLINE const UMonsterCombatSystem* GetCombatSystem() const { return CombatSystem; }
	void ApplyDamage(FHitResult& HitResult);
	void SetCurrentAttackDefinition(const FMonsterAttackDefinition& AttackDefinition);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(UAnimMontage* Montage);
	//only Server Callable
	void PlayMontageAndSubscribeEndDelegate(UAnimMontage* Montage, FOnMontageEnded& EndDelegate);

	void StartAttack();
	void EndAttack();

	virtual EEntityType GetEntityType() override;
	virtual void TakeDamage(FDamageContext& Context) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BT;
	UPROPERTY(EditAnywhere)
	UMonsterData* Data;

private:
	UPROPERTY()
	AMonsterController* MonsterController;
	UPROPERTY()
	UMonsterCombatSystem* CombatSystem;
	UPROPERTY()
	UMonsterAttackSystem* AttackSystem;
	
	const FMonsterAttackDefinition* CurrentAttackDefinition;
};
