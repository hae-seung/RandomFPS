// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/PlayerInteractSystem.h"

#include "GameScene/Player/PlayerCharacter.h"

UPlayerInteractSystem::UPlayerInteractSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}


void UPlayerInteractSystem::BeginPlay()
{
	Super::BeginPlay();
}

bool UPlayerInteractSystem::IsInteracting()
{
	return bIsInteracting;
}

void UPlayerInteractSystem::StartInteract(
	UAnimMontage* Montage,
	float EndTime,
	float WaitTime,
	UTexture2D* InteractImage)
{
	bIsInteracting = true;
	
	PlayInteractMontage(Montage);
	SetInteractEndTime(EndTime, WaitTime, InteractImage);

	Multicast_PlayInteractMontage(Montage);
	Client_SetInteractEndTime(EndTime, WaitTime, InteractImage);
}


void UPlayerInteractSystem::PlayInteractMontage(UAnimMontage* Montage)
{
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(Montage)
		{
			APC->PlayAnimMontage(Montage);
		}
	}
}

void UPlayerInteractSystem::SetInteractEndTime(float EndTime, float WaitTime, UTexture2D* Icon)
{
	OnInteractStart.Broadcast(EndTime, WaitTime, Icon);
}

void UPlayerInteractSystem::StopInteractMontage()
{
	bIsInteracting = false;
	
	StopMontage();
	Multicast_StopAnimMontage();
}

void UPlayerInteractSystem::SetMontageJumpEnd()
{
	bIsInteracting = false;

	JumpMontageEnd();
	Multicast_SetMontageJumpEnd();
}

void UPlayerInteractSystem::StopMontage()
{
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		APC->StopAnimMontage();
		OnInteractEnd.Broadcast();
	}
}

void UPlayerInteractSystem::JumpMontageEnd()
{
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(UAnimInstance* AnimInstance = APC->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_JumpToSection("End");
			OnInteractEnd.Broadcast();
		}
	}
}


void UPlayerInteractSystem::Multicast_PlayInteractMontage_Implementation(UAnimMontage* Montage)
{
	if(GetOwner()->HasAuthority())
		return;
	
	PlayInteractMontage(Montage);
}

void UPlayerInteractSystem::Multicast_StopAnimMontage_Implementation()
{
	if(GetOwner()->HasAuthority())
		return;
	
	StopMontage();
}

void UPlayerInteractSystem::Client_SetInteractEndTime_Implementation(
	float EndTime, float WaitTime, UTexture2D* Icon)
{
	if(GetOwner()->HasAuthority())
		return;

	SetInteractEndTime(EndTime, WaitTime, Icon);
}

void UPlayerInteractSystem::Multicast_SetMontageJumpEnd_Implementation()
{
	if(GetOwner()->HasAuthority())
		return;
	
	JumpMontageEnd();
}
