// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/CombatStructHeader.h"
#include "PlayerStatSystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerHealthStatChanged, const FPlayerHealthStat&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerCombatStatChanged, const FPlayerCombatStat&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUtilityStatChanged, const FPlayerUtilityStat&);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerStatSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnPlayerHealthStatChanged OnPlayerHealthStatChanged;
	FOnPlayerCombatStatChanged OnPlayerCombatStatChanged;
	FOnPlayerUtilityStatChanged OnPlayerUtilityStatChanged;

	
public:	
	UPlayerStatSystem();
	FORCEINLINE const FPlayerHealthStat& GetHealthStat() const { return HealthStat; }
	FORCEINLINE const FPlayerCombatStat& GetCombatStat() const { return CombatStat; }
	FORCEINLINE const FPlayerUtilityStat& GetUtilityStat() const { return UtilityStat; }

	void InitDelegates() const;
	void Revive();
	void ModifyHp(float Delta);
	
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


private:

	UFUNCTION()
	void OnRep_HealthStat();
	UFUNCTION()
	void OnRep_CombatStat();
	UFUNCTION()
	void OnRep_UtilityStat();
};
