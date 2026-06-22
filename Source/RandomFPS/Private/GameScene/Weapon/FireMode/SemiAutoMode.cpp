// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/FireMode/SemiAutoMode.h"


#include "GameScene/PoolManager.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Weapon/Bullet.h"
#include "GameScene/Weapon/Gun.h"




void USemiAutoMode::Shot(TSubclassOf<ABullet> BP_Bullet,
                         AMyPlayerController* Controller,
                         AGun* GunActor,
                         bool HasBullet)
{
	if(IsShooting || !bInit ||GunActor->GetMagAmmo() <= 0)
		return;

	IsShooting = true;

	Super::Shot(BP_Bullet, Controller, GunActor, HasBullet);

	//프로젝타일 발사
	//1.불렛 소환 (과 동시에 Instigator, owner 설정 끝남)
	ABullet* Bullet = Cast<ABullet>(GetWorld()->
		GetSubsystem<UPoolManager>()->
		Server_GetActor(BP_Bullet, GunActor->GetOwner()));

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
	Bullet->Shot(ShootDir, GunActor->GetGunInstance()->GetBulletSpeed(), HasBullet);
	GunActor->Fire();
}
