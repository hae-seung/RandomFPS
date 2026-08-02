// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/PlayerInteractSystem.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameScene/InteractableObject/InteractableObject.h"
#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

UPlayerInteractSystem::UPlayerInteractSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UPlayerInteractSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerInteractSystem, bIsInteracting);
}

void UPlayerInteractSystem::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(APC->IsLocallyControlled())
		{
			if(UCapsuleComponent* Capsule = APC->GetInteractCapsule())
			{
				Capsule->OnComponentBeginOverlap.AddDynamic(this, &UPlayerInteractSystem::DetectInteractableObject);
				Capsule->OnComponentEndOverlap.AddDynamic(this, &UPlayerInteractSystem::LostInteractableObject);
			}
		}
	}
}

void UPlayerInteractSystem::InputInteract()
{
	Server_InputInteract(CurNearInteractableObject);
}

void UPlayerInteractSystem::Server_InputInteract_Implementation(AInteractableObject* NearObject)
{
	if(bIsInteracting)
	{
		//강제중지
		StopInteractMontage();
	}
	else
	{
		if(IsValid(NearObject))
		{
			//todo: 상호작용이 끝나면 false로 바꿔주는 부분 필요함
			bIsInteracting = true;
			
			Client_ChangeCameraViewTarget(NearObject);
			NearObject->Interact(Cast<APlayerCharacter>(GetOwner()));
		}
	}
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
	if(WaitTime <= 0.f)
		return;
	
	OnInteractStart.Broadcast(EndTime, WaitTime, Icon);
	
	GetWorld()->GetTimerManager().SetTimer(
		InteractTimer,
		this, &UPlayerInteractSystem::SetMontageJumpEnd,
		WaitTime,
		false);
}

//상호작용을 도중에 중지시킴
void UPlayerInteractSystem::StopInteractMontage()
{
	bIsInteracting = false;
	
	StopMontage();
	Multicast_StopAnimMontage();
}

//상호작용이 끝나서 자동정지
void UPlayerInteractSystem::SetMontageJumpEnd()
{
	bIsInteracting = false;

	JumpMontageEnd();
	Multicast_SetMontageJumpEnd();
}

void UPlayerInteractSystem::StopMontage()
{
	if(GetOwner()->HasAuthority())
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		APC->StopAnimMontage();
		OnInteractEnd.Broadcast(false);
	}
}

void UPlayerInteractSystem::JumpMontageEnd()
{
	if(GetOwner()->HasAuthority())
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(UAnimInstance* AnimInstance = APC->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_JumpToSection("End");
			OnInteractEnd.Broadcast(true);
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

void UPlayerInteractSystem::DetectInteractableObject(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(AInteractableObject* Object = Cast<AInteractableObject>(OtherActor))
	{
		InteractableObjectsSet.Add(Object);
		UpdateNearestObject();
	}
}

void UPlayerInteractSystem::LostInteractableObject(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(AInteractableObject* Object = Cast<AInteractableObject>(OtherActor))
	{
		InteractableObjectsSet.Remove(Object);
		UpdateNearestObject();
	}
}

void UPlayerInteractSystem::UpdateNearestObject()
{
	AInteractableObject* NewObject = FindNearestObject();

	if(NewObject == CurNearInteractableObject)
		return;

	if(IsValid(CurNearInteractableObject))
	{
		CurNearInteractableObject->SetInteractState(false);
	}

	CurNearInteractableObject = NewObject;

	if(IsValid(CurNearInteractableObject) && !bIsInteracting)
	{
		CurNearInteractableObject->SetInteractState(true);
	}
}

AInteractableObject* UPlayerInteractSystem::FindNearestObject()
{
	AInteractableObject* Closet = nullptr;
	float MinDistance = TNumericLimits<float>::Max();

	FVector PlayerLoc = GetOwner()->GetActorLocation();
	
	for(AInteractableObject* Object: InteractableObjectsSet)
	{
		if(!IsValid(Object))
			continue;

		float Distance = FVector::DistSquared(
			PlayerLoc, Object->GetActorLocation());

		if(Distance < MinDistance)
		{
			Closet = Object;
			MinDistance = Distance;
		}
	}

	return Closet;
}

void UPlayerInteractSystem::Client_ChangeCameraViewTarget_Implementation(AInteractableObject* NearObject)
{
	UCameraComponent* Cam = NearObject->GetCamera();
	if(IsValid(Cam))
	{
		APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner());
		APC->ChangeCameraViewTarget(NearObject, CameraBlendTime);
	}
}


void UPlayerInteractSystem::OnRep_bIsInteracting()
{
	if(bIsInteracting && IsValid(CurNearInteractableObject))
	{
		CurNearInteractableObject->SetInteractState(false);
	}
	else if(!bIsInteracting && IsValid(CurNearInteractableObject))
	{
		CurNearInteractableObject->SetInteractState(true);
	}
	
}
