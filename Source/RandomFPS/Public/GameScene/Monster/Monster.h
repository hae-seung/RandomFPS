// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameScene/Poolable.h"
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
class RANDOMFPS_API AMonster :
public ACharacter, public IDamageable, public IPoolable
{
	GENERATED_BODY()

public:
	AMonster();
	UBehaviorTree* GetBT() const;
	FORCEINLINE UMonsterData* GetData() const { return Data; }
	FORCEINLINE const FMonsterAttackDefinition* GetCurAttackDefinition() const { return CurrentAttackDefinition; }
	FORCEINLINE UMonsterCombatSystem* GetCombatSystem() const { return CombatSystem; }
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	void ApplyDamage(FHitResult& HitResult);
	void SetCurrentAttackDefinition(const FMonsterAttackDefinition& AttackDefinition);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(UAnimMontage* Montage);
	//only Server Callable
	void PlayMontageAndSubscribeEndDelegate(UAnimMontage* Montage, FOnMontageEnded& EndDelegate);

	void StartAttack();
	void EndAttack();

	virtual EEntityType GetEntityType() override;
	//맞음
	virtual void TakeDamage(FDamageContext& Context) override;
	virtual bool GetIsDead() override;



	//IPoolable
	virtual void Acquire(AActor* NewOwner) override;
	virtual void Release() override;
	virtual bool IsActive() const override;

	//FromSpawner
	//현재 라운드를 받아서 해당 라운드 진행도 만큼 강화되도록함 
	void SetMonsterReinforceData(int CurSpawnRound);
	void NotDeadButRequestReleasePool(bool bState);
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BT;
	UPROPERTY(EditAnywhere)
	UMonsterData* Data;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_bIsDead)
	bool bIsDead;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_bIsActive)
	bool bIsActive;

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

	void SetDeadState(bool bState);
	UFUNCTION()
	void OnRep_bIsDead();
	UFUNCTION()
	void OnRep_bIsActive();
};
