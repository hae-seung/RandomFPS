// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "PlayerWeapon.generated.h"

class AMyPlayerController;
class APlayerCharacter;
class APreviewGun;
class AGun;
class UPartsItem;
class UGunItem;
class UCharacterLinkedAnimLayer;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UPlayerWeapon : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	bool bHasWeapon;
	
	
public:	
	UPlayerWeapon();
	UGunItem* GetCurrentEquipGun();
	UPartsItem* GetCurrentEquipParts(EPartsType PartsType);
	void UnEquipParts(EPartsType PartsType);
	bool EquipParts(UPartsItem* PartsItem);
	

	void UnEquipGun();
	bool EquipGun(UGunItem* GunInstance);
	void ZoomShort(bool bZoomState);
	FTransform GetLeftHandIKTransform();

	UFUNCTION(Server, Reliable)
	void Server_Shot();
	UFUNCTION(Server, Reliable)
	void Server_StopShot();

	void Reload();
	bool CanReload();

protected:
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_GunActor)
	AGun* GunActor;

	//오직 클라이언트 로컬에만 존재
	APreviewGun* PreviewGun;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APreviewGun> BP_PreviewGun;
	UPROPERTY(EditAnywhere)
	FTransform PreviewGunSpawnTransform;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCharacterLinkedAnimLayer> UnArmedLayer;
	
	//샷건 스나이퍼 애니메이션이 없어서 라이플 애님레이어로 통합
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	TSubclassOf<UCharacterLinkedAnimLayer> AnimLayer;


private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SpawnPreviewGun(APlayerCharacter* LocalPlayer);
	void UpdatePreviewGun();

	UFUNCTION(Client,Reliable)
	void Client_UpdateParts(UPartsItem* PartsItem);
	UFUNCTION(Client,Reliable)
	void Client_UnEquipParts(EPartsType PartsType);
	UFUNCTION()
	void OnRep_GunActor();
};
