// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/Bullet.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameScene/PoolManager.h"
#include "GameScene/Manager/ImpactManager/ImpactManager.h"
#include "Interface/Damageable.h"
#include "Interface/Killable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

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

void ABullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABullet, bIsActive);
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

	//맞은게 생물인경우
	if(IDamageable* Damageable = Cast<IDamageable>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("본 네임 : %s"), *Hit.BoneName.ToString());

		IKillable* Killable = Cast<IKillable>(GetOwner());
		Killable->ApplyDamage(Damageable, Hit.Location, Hit.BoneName, bIsRealBullet);
		
		ClearTimer();
		Release();
		return;
	}

	
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

	ProjectileMovementComponent->Activate(true);
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	ProjectileMovementComponent->Activate(true);
	ProjectileMovementComponent->SetUpdatedComponent(Collision);
	ProjectileMovementComponent->StopMovementImmediately();
}

void ABullet::Release()
{
	bIsActive = false;

	ProjectileMovementComponent->StopMovementImmediately();
	
	SetInstigator(nullptr);
	SetOwner(nullptr);
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
	ClearTimer();

	UPoolManager* PoolManager = GetWorld()->GetSubsystem<UPoolManager>();
	PoolManager->Server_ReleaseActor(this);
}

bool ABullet::IsActive() const
{
	return bIsActive;
}

void ABullet::Shot(const FVector& Direction, float Speed, bool bHasBullet)
{
	//5초뒤 물체에 안맞으면 풀에 반환
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandler,
		this,
		&ABullet::Release,
		5.0f,
		false);

	bIsRealBullet = bHasBullet;
	ProjectileMovementComponent->UpdateComponentVelocity();
	ProjectileMovementComponent->Velocity = Direction * Speed;
}


void ABullet::OnRep_bIsActive()
{
	SetActorHiddenInGame(!bIsActive);
}
