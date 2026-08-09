// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractSystem.generated.h"

class AInteractableObject;
//끝나는시간, 상호작용 시간, 상호작용 대상 아이콘
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInteractStart, float, float, UTexture2D*);

//상호작용 UI 끄는 용도
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractEnd, bool);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerInteractSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInteractStart OnInteractStart;
	FOnInteractEnd OnInteractEnd;
	

public:	
	UPlayerInteractSystem();
	void InputInteract();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void StartInteract(UAnimMontage* Montage, float EndTime, float WaitTime, UTexture2D* InteractImage);
	void StopInteractMontage();
	void SetMontageJumpEnd();
	bool IsInteracting();

private:
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_bIsInteracting)
	bool bIsInteracting;
	UPROPERTY(EditAnywhere)
	float CameraBlendTime;
	FTimerHandle InteractTimer;

	UPROPERTY()
	TSet<AActor*> InteractableObjectsSet;
	UPROPERTY()
	AActor* CurNearInteractableObject;
	

private:
	void PlayInteractMontage(UAnimMontage* Montage);
	void SetInteractEndTime(float EndTime, float WaitTime, UTexture2D* Icon);
	void StopMontage();
	void JumpMontageEnd();
	AActor* FindNearestObject();
	void UpdateNearestObject();
	bool IsPlayingMontage();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayInteractMontage(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopAnimMontage();
	UFUNCTION(Client, Reliable)
	void Client_SetInteractEndTime(float EndTime, float WaitTime, UTexture2D* Icon);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMontageJumpEnd();

	UFUNCTION()
	void DetectInteractableObject(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	void LostInteractableObject(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	
	UFUNCTION()
	void OnRep_bIsInteracting();
	UFUNCTION(Server,Reliable)
	void Server_InputInteract(AActor* NearObject);
};
