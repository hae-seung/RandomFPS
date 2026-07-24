// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractSystem.generated.h"

//끝나는시간, 상호작용 시간, 상호작용 대상 아이콘
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInteractStart, float, float, UTexture2D*);

//상호작용 UI 끄는 용도
DECLARE_MULTICAST_DELEGATE(FOnInteractEnd);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerInteractSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInteractStart OnInteractStart;
	FOnInteractEnd OnInteractEnd;
	

public:	
	UPlayerInteractSystem();

protected:
	virtual void BeginPlay() override;

public:
	void StartInteract(UAnimMontage* Montage, float EndTime, float WaitTime, UTexture2D* InteractImage);
	void StopInteractMontage();
	void SetMontageJumpEnd();
	bool IsInteracting();

private:
	bool bIsInteracting;

private:
	void PlayInteractMontage(UAnimMontage* Montage);
	void SetInteractEndTime(float EndTime, float WaitTime, UTexture2D* Icon);
	void StopMontage();
	void JumpMontageEnd();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayInteractMontage(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopAnimMontage();
	UFUNCTION(Client, Reliable)
	void Client_SetInteractEndTime(float EndTime, float WaitTime, UTexture2D* Icon);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMontageJumpEnd();
};
