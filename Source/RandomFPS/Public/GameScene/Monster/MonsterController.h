// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameScene/Cards/StructHeader.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterController.generated.h"

class AMonster;
class UAISenseConfig;
class UMonsterData;
class UAISenseConfig_Sight;
class UBlackboardComponent;


UCLASS()
class RANDOMFPS_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsAttacking() const { return bIsAttacking; }
	

public:
	void CheckCanAttackTarget();
	void SetFlinchState(AActor* Attacker);
	void Attack();
	

protected:
	AMonsterController();
	virtual void OnPossess(APawn* InPawn) override;
	

private:
	UPROPERTY(EditAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditAnywhere)
	UAISenseConfig_Sight* SightConfig;

	// UPROPERTY()
	// TMap<TSubclassOf<UAISense>, TObjectPtr<UAISenseConfig>> Senses;

	UPROPERTY()
	AMonster* Monster;
	UPROPERTY()
	UMonsterData* MonsterData;
	UPROPERTY()
	UBlackboardComponent* BB;
	UPROPERTY()
	TSet<AActor*> Targets;
	UPROPERTY()
	AActor* CurrentTarget;
	
	bool bIsFlinching;
	bool bIsAttacking; //AttackTask를 끝내는 용도.
	FTimerHandle FlinchTimer;
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category="BlackBoardKey")
	FName TargetActorKey;
	UPROPERTY(EditAnywhere, Category="BlackBoardKey")
	FName LostTargetKey;
	UPROPERTY(EditAnywhere, Category="BlackBoardKey")
	FName LostLocationKey;
	UPROPERTY(EditAnywhere, Category="BlackBoardKey")
	FName CanAttackKey;
	UPROPERTY(EditAnywhere, Category="BlackBoardKey")
	FName FlinchKey;

private:
	void SetMonsterPerception(UMonsterData* Data);
	void InitSightPerception(const FAISightPerceptionInfo& SightInfo);
	void SetDominantSense();
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void ActorInSight(AActor* Actor);
	void ActorOutSight(AActor* Actor);
	AActor* GetTargetInTargets();
	void ChangeCurrentTarget(AActor* NewTargetActor);
	void PlayFlinchMontage();
	bool CheckTargetAlive();
	void RefreshTargets();

	UFUNCTION()
	void OnDeadEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnAttackEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnFlinchEnd(UAnimMontage* Montage, bool bInterrupted);
};
