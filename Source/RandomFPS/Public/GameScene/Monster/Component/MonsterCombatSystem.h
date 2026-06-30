// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/CombatStructHeader.h"
#include "Struct/MonsterStruct.h"
#include "MonsterCombatSystem.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMonsterHealthStatChanged, float, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterFlinched, AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterDead, bool);


class UMonsterData;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UMonsterCombatSystem : public UActorComponent
{
	GENERATED_BODY()
	
public:
	FOnMonsterHealthStatChanged OnMonsterHealthStatChanged;
	FOnMonsterFlinched OnMonsterFlinched;
	FOnMonsterDead OnMonsterDead;
	
public:	
	UMonsterCombatSystem();
	void Init(UMonsterData* Data);
	void ApplyDamage(FHitResult& HitResult); //데미지를 입히는 것
	float TakeDamage(FDamageContext& Context); //데미지를 받는 것
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_Hp);
	float Hp;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_MaxHp);
	float MaxHp;
	
	UPROPERTY()
	FMonsterStat Stat;
	

private:
	float CalculateAttackDamage(FDamageContext& AttackContext);
	float GetFinalDamage(FDamageContext& Context);
	void Dead(AActor* Attacker);
	
	UFUNCTION()
	void OnRep_Hp();
	UFUNCTION()
	void OnRep_MaxHp();
};
