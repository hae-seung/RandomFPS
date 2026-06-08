// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/CombatStructHeader.h"
#include "PlayerCombatSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerCombatSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerCombatSystem();
	void TakeDamage(FDamageContext& Context);

	
protected:
	virtual void BeginPlay() override;



private:
	float Hp;
	UPROPERTY(EditAnywhere)
	FPlayerStat Stat;

private:
	int CalculateGetDamage(FDamageContext& Context);
};
