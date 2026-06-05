// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/FireMode/FullAutoMode.h"

#include "GameScene/PoolManager.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Weapon/Bullet.h"
#include "GameScene/Weapon/Gun.h"

void UFullAutoMode::Shot(
	TSubclassOf<ABullet> BP_Bullet,
	AMyPlayerController* Controller,
	AGun* GunActor,
	bool HasBullet)
{
	if(IsShooting || !bInit ||GunActor->GetMagAmmo() <= 0)
		return;

	IsShooting = true;

	CachedBP_Bullet = BP_Bullet;
	CachedController = Controller;
	CachedGunActor = GunActor;
	CachedHasBullet = HasBullet;
	
	AutoShot();
	GunActor->GetWorld()->GetTimerManager().SetTimer(
		ShotTimer,
		this,
		&UFullAutoMode::AutoShot,
		FireBetTime,
		true);
}

void UFullAutoMode::StopShot()
{
	Super::StopShot();
	CachedGunActor->GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
}

void UFullAutoMode::AutoShot()
{
	if(CachedGunActor->GetMagAmmo() <= 0)
	{
		StopShot();
		return;
	}
	
	Super::Shot(
		CachedBP_Bullet,
		CachedController,
		CachedGunActor,
		CachedHasBullet);
	

	ABullet* Bullet = Cast<ABullet>(GetWorld()->
		GetSubsystem<UPoolManager>()->
		Server_GetActor(CachedBP_Bullet, CachedGunActor->GetOwner()));

	if(!Bullet)
	{
		UE_LOG(LogTemp,Warning, TEXT("No Bullet ! Cant Shot"));
		return;
	}

	//2.불렛 방향 설정
	const FRotator BulletRot = ShootDir.Rotation();
	Bullet->SetActorTransform(FTransform(BulletRot, MuzzleLoc));
	
	//3.Bullet의 ProjectileMovement설정
	//발사 날리기
	Bullet->Shot(ShootDir, CachedGunActor->GetGunInstance()->GetBulletSpeed());
	CachedGunActor->Fire();
}
