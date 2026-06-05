

#include "GameScene/Weapon/PreviewGun.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/ItemData/PartsItemData.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "GameScene/Weapon/Parts.h"
#include "UI/InventoryUI.h"
#include "UI/UIManager.h"

// Sets default values
APreviewGun::APreviewGun()
{
	bReplicates = false;
	PrimaryActorTick.bCanEverTick = false;
	SkeletalGunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMesh"));
	CaptureCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureCamera"));
}

void APreviewGun::Init(AMyPlayerController* Controller)
{
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(APC->IsLocallyControlled())
		{
			UTextureRenderTarget2D* RT = NewObject<UTextureRenderTarget2D>(this);
			RT->InitAutoFormat(2048, 1024);
			RT->UpdateResourceImmediate(true);
			
			CaptureCamera->TextureTarget = RT;
			PreviewRenderTarget = RT;
			
			Controller->GetUIManager()->GetInventoryUI()->SetGunRenderTarget(RT);
		}
	}
}


void APreviewGun::UpdateGunPreview(USkeletalMesh* GunSkin)
{
	SkeletalGunMeshComponent->SetSkeletalMesh(GunSkin);

	CaptureCamera->AttachToComponent(
		SkeletalGunMeshComponent,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("Preview"));
	
	CaptureCamera->CaptureScene();
}


void APreviewGun::UpdateParts(UPartsItem* PartsItem)
{
	EPartsType PartsType = PartsItem->GetPartsType();
	if (AParts* Parts = PartsMap.FindRef(PartsType))
	{
		Parts->Destroy();
	}
	
	TSubclassOf<AParts> BP_Parts = PartsItem->GetPartsItemData()->GetBP_Parts();
	FActorSpawnParameters Params;
	Params.Owner = this;
	FName SocketName = PartsItem->GetPartsItemData()->GetPartsSocketName();


	AParts* PartsActor = GetWorld()->SpawnActor<AParts>(BP_Parts, Params);
	PartsActor->AttachToComponent(
		SkeletalGunMeshComponent,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName);
	
	PartsMap.Add(PartsType, PartsActor);
	
	CaptureCamera->CaptureScene();
}

void APreviewGun::UnEquipParts(EPartsType PartsType)
{
	AParts* Parts = PartsMap.FindRef(PartsType);
	if(Parts)
	{
		Parts->Destroy();
	}
	CaptureCamera->CaptureScene();
}


