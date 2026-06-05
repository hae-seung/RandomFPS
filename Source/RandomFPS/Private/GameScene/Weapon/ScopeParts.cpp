// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/ScopeParts.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "Kismet/KismetRenderingLibrary.h"


AScopeParts::AScopeParts()
{
	bReplicates = true;
	SceneCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture2D"));
	SceneCamera->SetupAttachment(StaticMeshComponent);
	SceneCamera->bCaptureEveryFrame = false;
	SceneCamera->bCaptureOnMovement = false;
}

void AScopeParts::BeginPlay()
{
	Super::BeginPlay();
	
	MakeDynamicLens();
}

void AScopeParts::MakeDynamicLens()
{
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(APC->IsLocallyControlled())
		{
			RT = UKismetRenderingLibrary::CreateRenderTarget2D(
				this, 1024, 1024);

			LensMID = StaticMeshComponent->CreateDynamicMaterialInstance(0);
			if(LensMID)
			{
				LensMID->SetTextureParameterValue("Scope", RT);
			}
			
			SceneCamera->TextureTarget = RT;
		}
	}
}


void AScopeParts::ZoomStart()
{
	SceneCamera->CaptureScene();
	SceneCamera->bCaptureEveryFrame = true;
	SceneCamera->CaptureScene();
}

void AScopeParts::ZoomEnd()
{
	SceneCamera->bCaptureEveryFrame = false;
	UKismetRenderingLibrary::ClearRenderTarget2D(
		this,
		RT,
		FLinearColor::Black);
}
