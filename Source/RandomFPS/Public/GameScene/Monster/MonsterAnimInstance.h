// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"


class UCharacterMovementComponent;
class AMonster;

UCLASS()
class RANDOMFPS_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere ,BlueprintReadOnly, meta=(AllowPrivateAccess))
	AMonster* Monster;
	UPROPERTY(VisibleAnywhere ,BlueprintReadOnly, meta=(AllowPrivateAccess))
	float Speed;
};
