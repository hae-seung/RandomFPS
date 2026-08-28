// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Cards/CardData.h"
#include "UObject/NoExportTypes.h"
#include "CardAbility.generated.h"

class UCardData;
class APlayerCharacter;


UCLASS(Abstract, Blueprintable, EditInlineNew)
class RANDOMFPS_API UCardAbility : public UObject
{
	GENERATED_BODY()

public:
	const UCardData* GetCardData() const; 


public:
	virtual void SetData(UCardData* Data);
	virtual UCardAbility* Clone() PURE_VIRTUAL(UCardAbility::Clone, return nullptr;);
	virtual void OnAcquire(APlayerCharacter* Player) PURE_VIRTUAL(UCardAbility::OnAcquire, );
	virtual void OnRemove(APlayerCharacter* Player) PURE_VIRTUAL(UCardAbility::OnRemove, );

private:
	UPROPERTY()
	UCardData* CardData;
};
