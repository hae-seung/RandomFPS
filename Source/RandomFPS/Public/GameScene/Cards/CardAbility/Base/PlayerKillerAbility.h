// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/StructHeader.h"
#include "GameScene/Cards/CardAbility/Base/CardAbility.h"
#include "PlayerKillerAbility.generated.h"


//플레이어 처치시 영구적으로 공격력, 방어력 증가
class UPlayerStatSystem;

UCLASS()
class RANDOMFPS_API UPlayerKillerAbility : public UCardAbility
{
	GENERATED_BODY()

public:
	virtual UCardAbility* Clone() override;
	virtual void OnAcquire(APlayerCharacter* Player) override;
	virtual void OnRemove(APlayerCharacter* Player) override;


private:
	UPROPERTY()
	UPlayerStatSystem* PlayerStatSystem;
	UPROPERTY(EditAnywhere)
	TArray<FStatModifier> Modifiers;

private:
	void KillPlayer(AActor* A, AActor* B, bool bCritic);
};
