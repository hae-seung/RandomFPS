// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/CombatStructHeader.h"
#include "PlayerCombatSystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerHealthStatChanged, const FPlayerHealthStat&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerCombatStatChanged, const FPlayerCombatStat&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUtilityStatChanged, const FPlayerUtilityStat&);

DECLARE_MULTICAST_DELEGATE(FOnPlayerDead);
DECLARE_MULTICAST_DELEGATE(FOnPlayerRevive);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnReviveTimeChanged, int);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerCombatSystem : public UActorComponent
{
	GENERATED_BODY()

	
public:
	FOnPlayerHealthStatChanged OnPlayerHealthStatChanged;
	FOnPlayerCombatStatChanged OnPlayerCombatStatChanged;
	FOnPlayerUtilityStatChanged OnPlayerUtilityStatChanged;
	
	FOnPlayerDead OnPlayerDead;
	FOnPlayerRevive OnPlayerRevive;
	FOnReviveTimeChanged OnReviveTimeChanged;
	
public:	
	UPlayerCombatSystem();
	void TakeDamage(FDamageContext& Context);
	void SubScribeInit();
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_HealthStat)
	FPlayerHealthStat HealthStat;
	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_CombatStat)
	FPlayerCombatStat CombatStat;
	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_UtilityStat)
	FPlayerUtilityStat UtilityStat;
	
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_RemainReviveTime)
	int RemainReviveTime;
	FTimerHandle ReviveTimer;

private:
	int CalculateGetDamage(FDamageContext& Context);
	void StartReviveTimer();
	void CheckReviveTime();
	
	UFUNCTION()
	void OnRep_HealthStat();
	UFUNCTION()
	void OnRep_CombatStat();
	UFUNCTION()
	void OnRep_UtilityStat();

	
	UFUNCTION()
	void OnRep_RemainReviveTime();
};
