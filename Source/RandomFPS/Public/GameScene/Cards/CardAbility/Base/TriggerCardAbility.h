// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/StructHeader.h"
#include "GameScene/Cards/CardAbility/Base/CardAbility.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "TriggerCardAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RANDOMFPS_API UTriggerCardAbility : public UCardAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ECardTrigger Trigger;

	virtual void Execute(const FCardEventContext& Context) PURE_VIRTUAL(UTriggerCardAbility::Execute, );
};
