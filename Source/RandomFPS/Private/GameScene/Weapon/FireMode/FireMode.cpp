// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/FireMode/FireMode.h"

#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Weapon/Gun.h"

bool UFireMode::IsSupportedForNetworking() const
{
	return true;
}

void UFireMode::Init(int TrueDamage, UFireData* FireData)
{
	NormalTrueDamage = TrueDamage;
	Data = FireData;
	bInit = true;
}

void UFireMode::Shot(TSubclassOf<ABullet> BP_Bullet,
	AMyPlayerController* Controller,
	AGun* GunActor,
	bool HasBullet)
{
	FVector CameraLoc;
	FRotator CameraRot;
	Controller->GetPlayerViewPoint(CameraLoc, CameraRot);

	const FVector TraceEnd = CameraLoc + (CameraRot.Vector() * 100000.f);
	
	FCollisionQueryParams Params;
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(Controller->GetPawn());
	Params.AddIgnoredActor(GunActor);
	
	GunActor->GetWorld()->LineTraceSingleByChannel(
		Hit,
		CameraLoc,
		TraceEnd,
		ECC_Visibility,
		Params);
	
	TargetPoint = Hit.bBlockingHit ? Hit.ImpactPoint : TraceEnd;

	MuzzleLoc = GunActor->GetMesh()->GetSocketLocation(TEXT("Muzzle"));
	ShootDir = (TargetPoint - MuzzleLoc).GetSafeNormal();
}


void UFireMode::StopShot()
{
	IsShooting = false;
}
