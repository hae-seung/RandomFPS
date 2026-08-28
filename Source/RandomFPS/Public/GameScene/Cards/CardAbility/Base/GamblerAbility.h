// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/CardAbility/Base/CardAbility.h"
#include "GamblerAbility.generated.h"



//플레이어 처치시 랜덤으로 골드 획득

class UPlayerWalletSystem;

UCLASS()
class RANDOMFPS_API UGamblerAbility : public UCardAbility
{
	GENERATED_BODY()


public:
	virtual UCardAbility* Clone() override;
	virtual void OnAcquire(APlayerCharacter* Player) override;
	virtual void OnRemove(APlayerCharacter* Player) override;


private:
	UPROPERTY(EditAnywhere)
	int MinGold;
	UPROPERTY(EditAnywhere)
	int MaxGold;
	
	UPROPERTY()
	UPlayerWalletSystem* PlayerWallet;

private:
	void KillOtherPlayer(AActor* A, AActor* B, bool bIsCritic);
};
