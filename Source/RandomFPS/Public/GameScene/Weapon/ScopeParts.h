// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/Weapon/Parts.h"
#include "ScopeParts.generated.h"

/**
 * 
 */
UCLASS()
class RANDOMFPS_API AScopeParts : public AParts
{
	GENERATED_BODY()

public:
	void ZoomStart();
	void ZoomEnd();

	
protected:
	AScopeParts();
	virtual void BeginPlay() override;
	
	
private:
	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D* RT;
	UPROPERTY()
	UMaterialInstanceDynamic* LensMID;
	UPROPERTY(EditAnywhere)
	USceneCaptureComponent2D* SceneCamera;

private:
	void MakeDynamicLens();
};
