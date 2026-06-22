// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Damageable.h"
#include "Struct/MonsterStruct.h"
#include "Monster.generated.h"

class UHealthUI;
class UWidgetComponent;
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
	//쳐맞음
	virtual void TakeDamage(FDamageContext& Context) override;
	virtual bool GetIsDead() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BT;
	UPROPERTY(EditAnywhere)
	UMonsterData* Data;

	bool bIsDead;

private:
	UPROPERTY()
	AMonsterController* MonsterController;
	UPROPERTY()
	UMonsterCombatSystem* CombatSystem;
	UPROPERTY()
	UMonsterAttackSystem* AttackSystem;
	UPROPERTY(EditAnywhere)
	UWidgetComponent* HealthBarComp;
	
	const FMonsterAttackDefinition* CurrentAttackDefinition;


private:
	void Server_SpawnBlood(FVector& HitLocation);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnBlood(FVector HitLocation);
	void SpawnBlood(FVector HitLocation);

	void Server_SpawnDamageActor(FVector& HitLocation, float Damage, bool bIsCritic);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnDamageActor(FVector HitLocation, float Damage, bool bIsCritic);
	void SpawnDamageActor(FVector& HitLocation, float Damage, bool bIsCritic);
};
