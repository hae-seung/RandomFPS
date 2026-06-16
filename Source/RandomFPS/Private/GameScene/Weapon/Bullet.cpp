// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/Bullet.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameScene/Manager/ImpactManager/ImpactManager.h"
#include "GameScene/Monster/Monster.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class APlayerCharacter;

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	Collision->bReturnMaterialOnMove = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	if(!HasAuthority())
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABullet::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if(!HasAuthority())
		return;

	//todo : 맞은게 생물인지 무생물인지 확인 필요
	
	//Impact 실행
	EPhysicalSurface Surface = SurfaceType_Default;
	const UPhysicalMaterial* PhysicMat = Hit.PhysMaterial.Get();
	if(PhysicMat)
	{
		Surface = UPhysicalMaterial::DetermineSurfaceType(PhysicMat);
	}
	
	UImpactManager* ImpactManager = GetWorld()->GetSubsystem<UImpactManager>();
	ImpactManager->SpawnImpact(
		Surface,
		Hit.ImpactPoint,
		GetActorRotation());
	Multicast_PlayImpact(Surface, Hit.ImpactPoint);
	//

	//데칼 스폰
	const FRotator DecalRot = UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal);
	SpawnDecal(Hit.ImpactPoint, DecalRot);
	Multicast_SpawnDecal(Hit.ImpactPoint, DecalRot);
	//
	
	//반환
	ClearTimer();
	Release();
	//
}

void ABullet::Multicast_PlayImpact_Implementation(EPhysicalSurface Surface, FVector Location)
{
	if(HasAuthority())
		return;

	UImpactManager* ImpactManager = GetWorld()->GetSubsystem<UImpactManager>();
	ImpactManager->SpawnImpact(
		Surface,
		Location,
		GetActorRotation());
}

void ABullet::Multicast_SpawnDecal_Implementation(FVector Location, FRotator Rot)
{
	if(HasAuthority())
		return;

	SpawnDecal(Location, Rot);
}

void ABullet::SpawnDecal(FVector Location, FRotator Rot)
{
	UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		Decal_BulletHole,
		DecalSize,
		Location,
		Rot,
		DecalLifeSpan)->SetFadeScreenSize(0.0015f);
}

void ABullet::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
}

void ABullet::Acquire(AActor* NewOwner)
{
	bIsActive = true;

	SetInstigator(Cast<APawn>(NewOwner));
	SetOwner(NewOwner);
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void ABullet::Release()
{
	bIsActive = false;

	SetInstigator(nullptr);
	SetOwner(nullptr);
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
	ProjectileMovementComponent->StopMovementImmediately();
}

bool ABullet::IsActive() const
{
	return bIsActive;
}

void ABullet::Shot(const FVector& Direction, float Speed)
{
	//5초뒤 물체에 안맞으면 풀에 반환
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandler,
		this,
		&ABullet::Release,
		5.0f,
		false);

	ProjectileMovementComponent->Velocity = Direction * Speed;
}

