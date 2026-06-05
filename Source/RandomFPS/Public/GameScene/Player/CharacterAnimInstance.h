// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class APlayerCharacter;



UCLASS()
class RANDOMFPS_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerCharacter* OwningCharacter;
	
	UCharacterMovementComponent* OwningMovementComponent;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CameraPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ZSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCrouch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsReloading;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasGun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform LeftHandIKTransform;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector JointTargetLocation;
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;



private:
	float RawSpeed;
	FVector Velocity;
	FRotator ActorRot;
};
