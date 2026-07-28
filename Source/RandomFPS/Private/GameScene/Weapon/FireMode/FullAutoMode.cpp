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
	if(IsShooting || !bInit)
		return;

	IsShooting = true;

	CachedBP_Bullet = BP_Bullet;
	CachedController = Controller;
	CachedGunActor = GunActor;
	CachedHasBullet = HasBullet;

	//타이머 방식이 아니라 Gun에서는 계속 Shot 입력을 주고
	//총의 능력중 TimeBet을 기준으로 현재 시간과 비교하여 쏠 수 있는지 확인하기
	//총알이 모자란건 Gun에서 알아서 걸러줄거임
	//얘네는 일단 쏘는 역할이면 됨
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
	

	ABullet* Bullet = Cast<ABullet>(
		GetWorld()->GetSubsystem<UPoolManager>()->
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
	Bullet->Shot(ShootDir, CachedGunActor->GetGunInstance()->GetBulletSpeed(), CachedHasBullet);
	CachedGunActor->Fire();
}
