// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameFramework/Actor.h"
#include "GameScene/Cards/StructHeader.h"

#include "Gun.generated.h"

class UHitScanModeData;
class UNiagaraSystem;
class APlayerCharacter;
class USoundManager;
class UFireMode;
class AMyPlayerController;
class UPartsItem;
class AParts;
class UGunItem;
class UCameraComponent;

DECLARE_DELEGATE_OneParam(FOnTotalAmmoChanged, int32);
DECLARE_DELEGATE_OneParam(FOnMagAmmoChanged, int32);
DECLARE_DELEGATE_OneParam(FOnMagAmmoTypeChanged, bool);


UCLASS()
class RANDOMFPS_API AGun : public AActor
{
	GENERATED_BODY()

public:
	FOnTotalAmmoChanged OnTotalAmmoChanged;
	FOnMagAmmoChanged OnMagAmmoChanged;
	FOnMagAmmoTypeChanged OnMagAmmoTypeChanged;
	
public:	
	AGun();
	
	UGunItem* GetGunInstance() const;

	void ZoomQuick(bool bZoomState);
	//Server
	void Shot();
	void StopShot();
	//실제 총이 쏴졌음.
	void Fire();
	
	void SetGunInstance(UGunItem* NewGunInstance);
	void EquipParts(AParts* PartsActor, UPartsItem* PartsItem);
	void UnEquipParts(EPartsType PartsType);
	void Reload();
	bool CanReload();

	
	void Server_ShowHitScanChannel(FHitScanInfo& HitScanInfo);
	void Server_SpawnHitImpact(FHitScanImpactInfo& HitScanImpactInfo);
	
	
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return SkeletalMeshComponent; }
	FORCEINLINE int GetMagAmmo() const { return MagAmmo; }
	USkeletalMesh* GetGunSkin();
	FName GetGunSocketName();
	UAnimMontage* GetReloadMontage();
	
protected:
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_GunInstance)
	UGunItem* GunInstance;

	//FireSystemIndex가 변할때만 수정.(only Server)
	UPROPERTY()
	TObjectPtr<UFireMode> CurrentFireSystem;
	//GunInstance 내부의 FireMode(Replicate대상) 확인
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_FireSystemIndex)
	int32 FireSystemIndex = 0;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnRep_Owner() override;

	
private:
	UPROPERTY()
	USoundManager* SoundManager;
	UPROPERTY()
	APlayerCharacter* APC;
	UPROPERTY()
	AMyPlayerController* Controller;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(EditAnywhere)
	UCameraComponent* SightCamera;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_CurCameraSocketName)
	FName CurCameraSocketName;
	UPROPERTY(EditAnywhere)
	FName DefaultIronSight = "IronSight";
	UPROPERTY(EditAnywhere)
	FName GunSocketName;

	UPROPERTY(Replicated)
	AParts* Rail;
	UPROPERTY(Replicated)
	AParts* Muzzle;
	UPROPERTY(Replicated)
	AParts* Magazine;

	UPROPERTY(Replicated, ReplicatedUsing=OnRep_MagAmmo)
	int32 MagAmmo;
	//인벤속 실탄 갯수
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_TotalAmmo)
	int32 TotalAmmo;
	//현재 mag에 장전된 총알이 공포탄이 아닌 실탄인지.
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_bIsRealBullet)
	bool bIsRealBullet;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void TryInit();
	
	void SetGunState();
	void SetGunAnimLayer();

	
	
	void ChangeCameraPos(FName SocketName);
	void BindCombatUI();
	UFUNCTION()
	void OnMagItemAdd(FName ItemId, int Amount);
	
	UFUNCTION()
	void OnRep_GunInstance();
	UFUNCTION()
	void OnRep_CurCameraSocketName();
	UFUNCTION()
	void OnRep_MagAmmo();
	UFUNCTION()
	void OnRep_TotalAmmo();
	UFUNCTION()
	void OnRep_bIsRealBullet();
	UFUNCTION()
	void OnRep_FireSystemIndex();

	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ShowHitScanChannel(FHitScanInfo HitScanInfo);
	void ShowHitScanChannel(FHitScanInfo& HitScanInfo);
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayShotEffect() const;
	void PlayShotEffect() const;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnHitImpact(FHitScanImpactInfo HitScanImpactInfo);
	void SpawnHitImpact(FHitScanImpactInfo& HitScanImpactInfo);
	
	void PlayEquipGunSound();
	UFUNCTION(Unreliable, NetMulticast)
	void Multicast_PlayFireSound();
	void PlayFireSound();
};

