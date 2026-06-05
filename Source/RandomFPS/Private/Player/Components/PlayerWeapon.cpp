// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/PlayerWeapon.h"

#include "Camera/CameraComponent.h"
#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/AnimInstance/CharacterLinkedAnimLayer.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemData/PartsItemData.h"
#include "GameScene/Player/ItemData/PartsData/RailPartsData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "GameScene/Player/ItemInstance/PartsItem/RailPartsItem.h"
#include "GameScene/Weapon/Gun.h"
#include "GameScene/Weapon/Parts.h"
#include "GameScene/Weapon/PreviewGun.h"
#include "Net/UnrealNetwork.h"


UPlayerWeapon::UPlayerWeapon()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}


void UPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		APC->GetMesh()->LinkAnimClassLayers(UnArmedLayer);

		if(APC->IsLocallyControlled())
		{
			SpawnPreviewGun(APC);
		}
	}
}

void UPlayerWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UPlayerWeapon, GunActor);
	DOREPLIFETIME(UPlayerWeapon, bHasWeapon);
}

void UPlayerWeapon::SpawnPreviewGun(APlayerCharacter* LocalPlayer)
{
	FActorSpawnParameters Params;
	Params.Owner = LocalPlayer;
	
	PreviewGun = GetWorld()->SpawnActor<APreviewGun>(
		BP_PreviewGun,
		PreviewGunSpawnTransform,
		Params);

	PreviewGun->Init(Cast<AMyPlayerController>(LocalPlayer->GetController()));
}

//local
void UPlayerWeapon::ZoomShort(bool bZoomState)
{
	if(IsValid(GunActor))
	{
		GunActor->ZoomQuick(bZoomState);
	}
}

void UPlayerWeapon::Server_Shot_Implementation()
{
	if(!GunActor)
		return;

	GunActor->Shot();
}

void UPlayerWeapon::Server_StopShot_Implementation()
{
	if(!GunActor)
		return;

	GunActor->StopShot();
}

bool UPlayerWeapon::CanReload()
{
	if(!IsValid(GunActor) || !GunActor->CanReload())
		return false;

	return true;
}

void UPlayerWeapon::Reload()
{
	GunActor->Reload();
}



FTransform UPlayerWeapon::GetLeftHandIKTransform()
{
	if(!GunActor)
		return FTransform::Identity;
	
	const FTransform LeftHandSocketWorld = GunActor->GetMesh()->GetSocketTransform(
		TEXT("LH"), RTS_World);

	return LeftHandSocketWorld;
}

UGunItem* UPlayerWeapon::GetCurrentEquipGun()
{
	if(GunActor == nullptr)
		return nullptr;
	
	return GunActor->GetGunInstance();
}

UPartsItem* UPlayerWeapon::GetCurrentEquipParts(EPartsType PartsType)
{
	UGunItem* Gun = GunActor->GetGunInstance();

	//총이 해당 파츠타입 장착이 불가능할때
	if(!Gun->CheckPartsType(PartsType))
		return nullptr;

	return Gun->GetCurrentEquipParts(PartsType);
}

//server
bool UPlayerWeapon::EquipGun(UGunItem* GunInstance)
{
	//액터 소환 후 Instance할당
	TSubclassOf<AGun> BP_NewGun = GunInstance->GunItemData->BP_Gun;
	if(BP_NewGun == nullptr)
		return false;
	
	APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner());
	if(!APC)
	{
		return false;
	}

	FActorSpawnParameters Params;
	Params.Owner = APC;
	
	GunActor = GetWorld()->SpawnActor<AGun>(BP_NewGun, Params);
	FName SocketName = GunActor->GetGunSocketName();
		
	GunActor->AttachToComponent(
		APC->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName);

	GunActor->SetGunInstance(GunInstance);

	bHasWeapon = true;
	
	if(APC->IsLocallyControlled())
	{
		UpdatePreviewGun();
	}
	
	
	return true;
}

void UPlayerWeapon::UnEquipGun()
{
	if(GunActor != nullptr)
	{
		GunActor->Destroy();
		GunActor = nullptr;
		bHasWeapon = false;
	}
}


bool UPlayerWeapon::EquipParts(UPartsItem* PartsItem)
{
	//파츠 액터 소환 후 AGun에 장착.
	TSubclassOf<AParts> BP_Parts = PartsItem->GetPartsItemData()->GetBP_Parts();
	if(!IsValid(BP_Parts))
		return false;

	APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner());
	if(!APC)
	{
		return false;
	}

	FActorSpawnParameters Params;
	Params.Owner = APC;
	FName SocketName = PartsItem->GetPartsItemData()->GetPartsSocketName();

	AParts* PartsActor = GetWorld()->SpawnActor<AParts>(BP_Parts, Params);
	PartsActor->AttachToComponent(
		GunActor->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName);

	
	GunActor->EquipParts(PartsActor, PartsItem);
	
	if(APC->IsLocallyControlled())
	{
		PreviewGun->UpdateParts(PartsItem);
	}
	else
	{
		Client_UpdateParts(PartsItem);
	}
	
	return true;
}

void UPlayerWeapon::UnEquipParts(EPartsType PartsType)
{
	GunActor->UnEquipParts(PartsType);

	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(APC->IsLocallyControlled())
		{
			PreviewGun->UnEquipParts(PartsType);
		}
		else
		{
			Client_UnEquipParts(PartsType);
		}
	}
}

void UPlayerWeapon::OnRep_GunActor()
{
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(APC->IsLocallyControlled())
		{
			UpdatePreviewGun();
		}
	}
}


void UPlayerWeapon::UpdatePreviewGun()
{
	if(!PreviewGun)
	{
		APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner());
		SpawnPreviewGun(APC);
	}
	PreviewGun->UpdateGunPreview(GunActor->GetGunSkin());
}

void UPlayerWeapon::Client_UpdateParts_Implementation(UPartsItem* PartsItem)
{
	PreviewGun->UpdateParts(PartsItem);
}

void UPlayerWeapon::Client_UnEquipParts_Implementation(EPartsType PartsType)
{
	PreviewGun->UnEquipParts(PartsType);
}
