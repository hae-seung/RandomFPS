// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/StructHeader.h"
#include "GameScene/Cards/CardAbility/Base/CardAbility.h"
#include "CriticalHitAbility.generated.h"


class UPlayerStatSystem;
//치명타(헤드샷) 적중시 공격력 증가(사망시 초기화)
UCLASS()
class RANDOMFPS_API UCriticalHitAbility : public UCardAbility
{
	GENERATED_BODY()


public:
	virtual UCardAbility* Clone() override;
	virtual void OnAcquire(APlayerCharacter* Player) override;
	virtual void OnRemove(APlayerCharacter* Player) override;

private:
	UPROPERTY(EditAnywhere)
	FStatModifier PlusAttack;

	float TotalPlusAmount;


	UPROPERTY()
	UPlayerStatSystem* PlayerStatSystem;

	
private:
	void ClearPlusAttack();
	void HitCritical(bool bIsCritic);
};
