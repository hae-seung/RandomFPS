// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStatSystem.h"
#include "Components/ActorComponent.h"
#include "Struct/CombatStructHeader.h"
#include "PlayerCombatSystem.generated.h"


class IDamageable;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnReviveTimeChanged, int);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitSuccess, bool);

DECLARE_MULTICAST_DELEGATE(FOnPlayerDead);
DECLARE_MULTICAST_DELEGATE(FOnPlayerRevive);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerCombatSystem : public UActorComponent
{
	GENERATED_BODY()

	
public:
	// FOnPlayerHealthStatChanged OnPlayerHealthStatChanged;
	// FOnPlayerCombatStatChanged OnPlayerCombatStatChanged;
	// FOnPlayerUtilityStatChanged OnPlayerUtilityStatChanged;
	
	FOnPlayerDead OnPlayerDead;
	FOnPlayerRevive OnPlayerRevive;
	FOnReviveTimeChanged OnReviveTimeChanged;

	FOnHitSuccess OnHitSuccess;
	
public:	
	UPlayerCombatSystem();
	void SetComponents(UPlayerStatSystem* PlayerStatSystem);
	void TakeDamage(FDamageContext& Context);
	//void SubScribeInit();
	void ApplyDamageToTarget(IDamageable* Target, FVector HitLocation, FName BoneName, bool bIsRealBullet);
	
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
	// UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_HealthStat)
	// FPlayerHealthStat HealthStat;
	// UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_CombatStat)
	// FPlayerCombatStat CombatStat;
	// UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_UtilityStat)
	// FPlayerUtilityStat UtilityStat;
	
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_RemainReviveTime)
	int RemainReviveTime;
	FTimerHandle ReviveTimer;

	UPROPERTY()
	UPlayerStatSystem* StatSystem;
	
	UPROPERTY()
	TMap<AActor*, float> HitMePlayers;
	UPROPERTY(EditAnywhere)
	float AssistTime = 4.5f;

private:
	void Dead(AActor* Attacker, bool bIsCritical);
	
	int CalculateGetDamage(FDamageContext& Context);
	float CalculateAttackDamage(float Damage, bool bIsCritic, const FPlayerCombatStat& CombatStat);
	
	void StartReviveTimer();
	void CheckReviveTime();
	
	// UFUNCTION()
	// void OnRep_HealthStat();
	// UFUNCTION()
	// void OnRep_CombatStat();
	// UFUNCTION()
	// void OnRep_UtilityStat();

	
	UFUNCTION()
	void OnRep_RemainReviveTime();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HitSuccess(bool bIsCritical);
	UFUNCTION(Client, Reliable)
	void Client_Dead();
	
};
