// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "UObject/Interface.h"
#include "Struct/CombatStructHeader.h"

#include "Damageable.generated.h"


UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};


class RANDOMFPS_API IDamageable
{
	GENERATED_BODY()

	
public:
	virtual EEntityType GetEntityType() = 0;
	virtual void TakeDamage(FDamageContext& Context) = 0;
	virtual bool GetIsDead() = 0;
};
