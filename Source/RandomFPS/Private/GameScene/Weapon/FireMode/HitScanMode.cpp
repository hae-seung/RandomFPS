// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/FireMode/HitScanMode.h"

#include "GameScene/Cards/StructHeader.h"
#include "GameScene/Weapon/Gun.h"
#include "GameScene/Weapon/FireMode/HitScanModeData.h"
#include "Kismet/KismetMathLibrary.h"

void UHitScanMode::Init(int TrueDamage, UFireData* FireData)
{
	HitScanData = Cast<UHitScanModeData>(FireData);
	Super::Init(TrueDamage, FireData);
}

void UHitScanMode::Shot(
	TSubclassOf<ABullet> BP_Bullet,
	AMyPlayerController* Controller,
	AGun* GunActor,
	bool HasBullet)
{
	if(IsShooting || !bInit ||GunActor->GetMagAmmo() <= 0)
		return;

	IsShooting = true;
	
	Super::Shot(BP_Bullet, Controller, GunActor, HasBullet);
	
	FVector EndLoc = TargetPoint;
	if(Hit.bBlockingHit)
	{
		EndLoc = Hit.ImpactPoint;
		//맞은 대상이 몬스터이거나 사람이면 데미지 넣고 Impact, 데칼 소환 불필요
		//맞은 대상이 알아서 스폰할거임. 피 튀는 연출 등
		
		
		//맞은 대상이 무생물이면 서버에서 스폰
		SpawnHitImpact(GunActor);
	}

	FHitScanInfo HitScanInfo(
		HitScanData->HitScanSmoke,
		MuzzleLoc,
		EndLoc,
		 HitScanData->StartName,
		HitScanData->EndName);
	
	GunActor->Server_ShowHitScanChannel(HitScanInfo);
	GunActor->Fire();
}


void UHitScanMode::SpawnHitImpact(AGun* GunActor)
{
	//표면 구하기
	EPhysicalSurface Surface = SurfaceType_Default;
	const UPhysicalMaterial* PhysicMat = Hit.PhysMaterial.Get();
	if(PhysicMat)
	{
		Surface = UPhysicalMaterial::DetermineSurfaceType(PhysicMat);
	}

	//스폰될 데칼의 회전방향 구하기
	const FRotator DecalRot = UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal);

	
	FHitScanImpactInfo HitScanImpactInfo(
		Hit.ImpactPoint,
		DecalRot,
		Surface,
		HitScanData->DecalSize,
		HitScanData->DecalLifeSpan,
		HitScanData->Decal_BulletHole);
	
	//넘기기
	GunActor->Server_SpawnHitImpact(HitScanImpactInfo);
}
