// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameScene/Poolable.h"
#include "Bullet.generated.h"


class UProjectileMovementComponent;
class USphereComponent;
class APlayerCharacter;

UCLASS()
class RANDOMFPS_API ABullet : public AActor, public IPoolable
{
	GENERATED_BODY()
	
public:	
	ABullet();
	virtual void Acquire(APlayerCharacter* NewOwner) override;
	UFUNCTION()
	virtual void Release() override;
	virtual bool IsActive() const override;

	void Shot(const FVector& Direction, float Speed);
	
protected:
	virtual void BeginPlay() override;


private:
	bool bIsActive;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere)
	USphereComponent* Collision;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	
	UPROPERTY(EditAnywhere)
	UMaterialInstance* Decal_BulletHole;
	UPROPERTY(EditAnywhere)
	FVector DecalSize;
	UPROPERTY(EditAnywhere)
	float DecalLifeSpan;

	
	UPROPERTY()
	FTimerHandle TimerHandler;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	void SpawnDecal(FVector Location, FRotator Rot);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnDecal(FVector Location, FRotator Rot);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayImpact(EPhysicalSurface Surface, FVector Location);
	
	void ClearTimer();
};
