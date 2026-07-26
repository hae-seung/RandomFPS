// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameScene/Cards/StructHeader.h"
#include "Struct/CombatStructHeader.h"
#include "PlayerStatSystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerHealthStatChanged, const FPlayerHealthStat&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerCombatStatChanged, const FPlayerCombatStat&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUtilityStatChanged, const FPlayerUtilityStat&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerEnergyChanged, int);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerStatSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnPlayerHealthStatChanged OnPlayerHealthStatChanged;
	FOnPlayerCombatStatChanged OnPlayerCombatStatChanged;
	FOnPlayerUtilityStatChanged OnPlayerUtilityStatChanged;

	FOnPlayerEnergyChanged OnPlayerEnergyChanged;

	
public:	
	UPlayerStatSystem();
	FORCEINLINE const FPlayerHealthStat& GetHealthStat() const { return HealthStat; }
	FORCEINLINE const FPlayerCombatStat& GetCombatStat() const { return CombatStat; }
	FORCEINLINE const FPlayerUtilityStat& GetUtilityStat() const { return UtilityStat; }

	void InitDelegates() const;
	void Revive();
	void ModifyHp(float Delta);
	
	void ApplyPortion(const FStatModifier& Modifier);
	
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

	//Energy는 특수 스탯
	FTimerHandle EnergyTimer;
	UPROPERTY(EditAnywhere)
	FDrinkStat DrinkStats[10];
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_EnergyIndex)
	int EnergyIndex;
	UPROPERTY(EditAnywhere)
	float DrinkApplyWaitTime = 30.f;

	float OriginWalkSpeed;
	UPROPERTY()
	APlayerCharacter* APC;
	
private:
	UFUNCTION()
	void OnRep_HealthStat();
	UFUNCTION()
	void OnRep_CombatStat();
	UFUNCTION()
	void OnRep_UtilityStat();
	UFUNCTION()
	void OnRep_EnergyIndex();

	void EnergyTick();
	void HandleEnergy(float Value);
	void HandleHp(float Value);
	void HandleMaxHp(float Value);
	void HandleWalkSpeed(float Value);
	void ApplyEnergyWalkSpeed();
};
