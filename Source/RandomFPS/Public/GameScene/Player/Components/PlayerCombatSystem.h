// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/CombatStructHeader.h"
#include "PlayerCombatSystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const FPlayerStat&);
DECLARE_MULTICAST_DELEGATE(FOnPlayerDead);
DECLARE_MULTICAST_DELEGATE(FOnPlayerRevive);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnReviveTimeChanged, int);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerCombatSystem : public UActorComponent
{
	GENERATED_BODY()

	
public:
	FOnStateChanged OnStateChanged;
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
	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_Stat)
	FPlayerStat Stat;
	
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_RemainReviveTime)
	int RemainReviveTime;
	FTimerHandle ReviveTimer;

private:
	int CalculateGetDamage(FDamageContext& Context);
	void StartReviveTimer();
	void CheckReviveTime();
	
	UFUNCTION()
	void OnRep_Stat();
	UFUNCTION()
	void OnRep_RemainReviveTime();
};
