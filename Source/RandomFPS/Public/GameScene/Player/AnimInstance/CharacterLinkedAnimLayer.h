// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterLinkedAnimLayer.generated.h"

class UCharacterAnimInstance;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UCharacterLinkedAnimLayer : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	UCharacterAnimInstance* GetCharacterAnimInstance() const;
};
