// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameScene/Player/PlayerCharacter.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if(OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	if(!OwningCharacter || !OwningMovementComponent)
		return;

	Velocity = OwningCharacter->GetVelocity();
	ActorRot = OwningCharacter->GetActorRotation();

	Direction = CalculateDirection(Velocity, ActorRot);
	RawSpeed = OwningCharacter->GetVelocity().Size2D();
	Speed = FMath::FInterpTo(
			Speed,
			RawSpeed,
			DeltaSeconds,
			8.0f);
	
	CameraPitch = OwningCharacter->AimPitch;
	bIsFalling = OwningMovementComponent->IsFalling();
	ZSpeed = OwningMovementComponent->Velocity.Z;
	bIsCrouch = OwningCharacter->bIsCrouched;
	bIsAiming = OwningCharacter->GetAiming();
	bIsReloading = OwningCharacter->GetReloading();
	bIsDead = OwningCharacter->IsDead();
	bHasGun = OwningCharacter->HasWeapon();
	LeftHandIKTransform = OwningCharacter->LeftHandIK;
}

