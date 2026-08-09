// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerWalletSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerWalletSystem : public UActorComponent
{
	GENERATED_BODY()


	
public:	
	UPlayerWalletSystem();
	int GetMoney() const;
	void SpendMoney(int OutAmount);
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_Money)
	int Money;


private:
	UFUNCTION()
	void OnRep_Money();
};
