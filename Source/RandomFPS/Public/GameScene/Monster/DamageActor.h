// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameScene/Poolable.h"
#include "DamageActor.generated.h"

class UDamageUI;
class UWidgetComponent;

UCLASS()
class RANDOMFPS_API ADamageActor : public AActor, public IPoolable
{
	GENERATED_BODY()
	
public:	
	ADamageActor();
	
	virtual void Acquire(AActor* NewOwner) override;
	virtual void Release() override;
	virtual bool IsActive() const override;
	
	void SetDamageText(float DamageAmount, bool bIsCritical);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


private:
	UPROPERTY(EditAnywhere)
	UWidgetComponent* DamageWidgetComponent;
	UPROPERTY(EditAnywhere)
	float LifeTime;
	UPROPERTY()
	FTimerHandle TimerHandle;
	UPROPERTY()
	UDamageUI* DamageUI;
	
	bool bIsActive = false;


	UPROPERTY()
	APlayerController* PC;

	FVector CameraLocation;
	FRotator CameraRotation;
	FRotator LookAtRot;

private:

	void StartReleaseTimer();
	void RotateToLocalCamera();
};
