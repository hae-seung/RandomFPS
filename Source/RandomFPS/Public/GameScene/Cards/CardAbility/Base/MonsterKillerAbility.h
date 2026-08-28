// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/StructHeader.h"
#include "GameScene/Cards/CardAbility/Base/CardAbility.h"
#include "MonsterKillerAbility.generated.h"


class UPlayerStatSystem;
//몬스터 처치시 최대체력 증가(영구)
UCLASS()
class RANDOMFPS_API UMonsterKillerAbility : public UCardAbility
{
	GENERATED_BODY()


public:
	virtual UCardAbility* Clone() override;
	virtual void OnAcquire(APlayerCharacter* Player) override;
	virtual void OnRemove(APlayerCharacter* Player) override;


private:
	UPROPERTY(EditAnywhere)
	FStatModifier PlusMaxHp;
	UPROPERTY()
	UPlayerStatSystem* PlayerStatSystem;
	
private:
	void KillMonster();
};
