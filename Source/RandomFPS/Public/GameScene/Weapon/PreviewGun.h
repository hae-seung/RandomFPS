// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "PreviewGun.generated.h"

class AMyPlayerController;
class AParts;
class UPartsItem;
class USpringArmComponent;

UCLASS()
class RANDOMFPS_API APreviewGun : public AActor
{
	GENERATED_BODY()
	
public:	
	APreviewGun();
	void Init(AMyPlayerController* Controller);
	void UpdateGunPreview(USkeletalMesh* GunSkin);
	void UpdateParts(UPartsItem* PartsItem);
	void UnEquipParts(EPartsType PartsType);

private:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalGunMeshComponent;
	UPROPERTY(EditAnywhere)
	USceneCaptureComponent2D* CaptureCamera;
	
	UTextureRenderTarget2D* PreviewRenderTarget;
	TMap<EPartsType, AParts*> PartsMap;
	
	FTimerHandle InitHandle;

private:
	void WaitInitUI();
};
