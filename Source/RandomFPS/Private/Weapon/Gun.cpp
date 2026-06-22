// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Weapon/Gun.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameScene/PoolManager.h"
#include "GameScene/Manager/ImpactManager/ImpactManager.h"
#include "GameScene/Manager/SoundManager/SoundManager.h"
#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/Inventory.h"
#include "GameScene/Player/ItemData/BulletItemData.h"
#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemData/PartsData/RailPartsData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "GameScene/Weapon/Parts.h"
#include "GameScene/Weapon/ScopeParts.h"
#include "GameScene/Weapon/FireMode/FireMode.h"
#include "GameScene/Weapon/FireMode/HitScanModeData.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/CombatUI.h"
#include "UI/UIManager.h"


AGun::AGun()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMeshComponent;

	SightCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SightCamera"));
	SightCamera->SetupAttachment(RootComponent);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	ChangeCameraPos(DefaultIronSight);
	
	APC = Cast<APlayerCharacter>(GetOwner());
	SoundManager = GetWorld()->GetSubsystem<USoundManager>();
	if(IsValid(APC))
	{
		Controller = Cast<AMyPlayerController>(APC->GetController());
	}
}

void AGun::OnRep_Owner()
{
	Super::OnRep_Owner();

	APC = Cast<APlayerCharacter>(GetOwner());
	SoundManager = GetWorld()->GetSubsystem<USoundManager>();
	Controller = Cast<AMyPlayerController>(APC->GetController());
	
	TryInit();
}

void AGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGun, GunInstance);
	DOREPLIFETIME(AGun, FireSystemIndex);
	
	DOREPLIFETIME(AGun, Rail);
	DOREPLIFETIME(AGun, Muzzle);
	DOREPLIFETIME(AGun, Magazine);

	DOREPLIFETIME(AGun, MagAmmo);
	DOREPLIFETIME(AGun, TotalAmmo);
	DOREPLIFETIME(AGun, bIsRealBullet);
}

//Local
void AGun::ZoomQuick(bool bZoomState)
{
	if(!Controller)
		return;

	AScopeParts* Scope = Cast<AScopeParts>(Rail);
	if(bZoomState)
	{
		Controller->SetViewTargetWithBlend(this, 0.1f);
		if(IsValid(Scope))
		{
			Scope->ZoomStart();
		}
	}
	else
	{
		if(IsValid(Scope))
		{
			Scope->ZoomEnd();
		}
	}
}



//Server
void AGun::Shot()
{
	if(!IsValid(CurrentFireSystem))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NoCurrentSystem"));
		return;
	}

	if(!IsValid(GunInstance))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NoGunInstance"));
		return;
	}
	
	if(!IsValid(GunInstance->GunItemData))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NoGunItemData"));
		return;
	}

	if(!IsValid(GunInstance->GunItemData->BulletItemData))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NoBPBUllet"));
		return;
	}
	if(!IsValid(Controller))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NoController"));
		return;
	}
	
	//todo : 현재 mag에 장전된 current_bp_bullet 넘겨줘야함
	//todo : 장전중이거나 mag가 비어있으면 return;
	//임시 shot
	CurrentFireSystem->Shot(
		GunInstance->GunItemData->BulletItemData->BP_Bullet,
		Controller,
		this,
		bIsRealBullet);
}

void AGun::StopShot()
{
	if(!CurrentFireSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("GunActor(StopShot) : No CurrentFireSystem"));
		return;
	}

	UE_LOG(LogTemp,Warning,TEXT("stopshot"));
	CurrentFireSystem->StopShot();
}

void AGun::Fire()
{
	MagAmmo--;
	
	OnMagAmmoChanged.ExecuteIfBound(MagAmmo);
	
	PlayShotEffect();
	Multicast_PlayShotEffect();

	PlayFireSound();
	Multicast_PlayFireSound();
}

//server
void AGun::Reload()
{
	//총알 업데이트.
	int RefillAmmo = GunInstance->GetMaxMagAmmo();
	int CurMagAmmo = MagAmmo;
	if(TotalAmmo > 0) //인벤토리에 실탄이 존재시
	{
		if(bIsRealBullet)
		{
			RefillAmmo -= CurMagAmmo;
		}
		else
		{
			CurMagAmmo = 0;
		}

		bIsRealBullet = true;
	}
	else 
	{
		//인벤토리에 실탄은 존재하지 않지만 장전 입력은 성공 했음
		//이 말은 그냥 공포탄으로 재장전임
		bIsRealBullet = false;
	}

	int OriginTotalAmmo = TotalAmmo;
	
	if(bIsRealBullet)
	{
		if(RefillAmmo > TotalAmmo)
		{
			CurMagAmmo += TotalAmmo;
			TotalAmmo = 0;
		}
		else
		{
			CurMagAmmo += RefillAmmo;
			TotalAmmo -= RefillAmmo;
		}

		if(IsValid(APC))
		{
			UInventory* Inventory = APC->GetInventory();
			Inventory->RemoveItemFromId(
				GunInstance->GunItemData->BulletItemData->GetItemId(),
				OriginTotalAmmo - TotalAmmo);
		}
	}
	else
	{
		CurMagAmmo = RefillAmmo;
		TotalAmmo = 0;
	}
	
	MagAmmo = CurMagAmmo;
	
	OnTotalAmmoChanged.ExecuteIfBound(TotalAmmo);
	OnMagAmmoChanged.ExecuteIfBound(MagAmmo);
	OnMagAmmoTypeChanged.ExecuteIfBound(bIsRealBullet);
}

bool AGun::CanReload()
{
	if(!IsValid(GunInstance))
		return false;

	if(bIsRealBullet)
	{
		if(TotalAmmo <= 0)
		{
			return MagAmmo <= 0;
		}
		
		return MagAmmo < (GunInstance->GetMaxMagAmmo());
	}
	else
	{
		//공포탄 장전상태
		
		//실탄이 없다면
		if(TotalAmmo <= 0)
		{
			return MagAmmo < (GunInstance->GetMaxMagAmmo());
		}

		//공포탄이 장전되어있지만 실탄이 1개라도 존재시 바로 실탄으로 재장전 가능
		return true;
	}
}

USkeletalMesh* AGun::GetGunSkin()
{
	return SkeletalMeshComponent->GetSkeletalMeshAsset();
}

FName AGun::GetGunSocketName()
{
	return GunSocketName;
}

UAnimMontage* AGun::GetReloadMontage()
{
	if(!GunInstance)
		return nullptr;

	return GunInstance->GunItemData->ReloadMontage;
}

UGunItem* AGun::GetGunInstance() const
{
	return GunInstance;
}

//server
void AGun::SetGunInstance(UGunItem* NewGunInstance)
{
	GunInstance = NewGunInstance;
	CurrentFireSystem = GunInstance->GetFireSystem(FireSystemIndex);
	
	if(IsValid(APC))
	{
		Controller = Cast<AMyPlayerController>(APC->GetController());
		UInventory* Inventory = APC->GetInventory();
		Inventory->OnItemAdd.AddUObject(this, &AGun::OnMagItemAdd);
		TotalAmmo = Inventory->GetItemTotalAmount(GunInstance->GunItemData->BulletItemData);
		
		if(APC->IsLocallyControlled())
		{
			BindCombatUI();
		}

		//sfx
		PlayEquipGunSound();
	}

	
	//SetGunState();
	SetGunAnimLayer();
}

void AGun::TryInit()
{
	if(!IsValid(APC) || !IsValid(GunInstance))
		return;

	//SetGunState();
	SetGunAnimLayer();
	
	if(APC->IsLocallyControlled())
	{
		Controller = Cast<AMyPlayerController>(APC->GetController());
		BindCombatUI();
	}
	
	PlayEquipGunSound();
}


void AGun::EquipParts(AParts* PartsActor, UPartsItem* PartsItem)
{
	EPartsType PartsType = PartsItem->GetPartsType();

	switch (PartsType)
	{
	case EPartsType::Rail:
		Rail = PartsActor;
		if(URailPartsData* RailData = Cast<URailPartsData>(PartsItem->GetPartsItemData()))
		{
			FName SocketName = RailData->GetCameraSocketName();
			CurCameraSocketName = SocketName;
			ChangeCameraPos(SocketName);
		}
		break;
	case EPartsType::Muzzle:
		Muzzle = PartsActor;
		break;
	case EPartsType::Magazine:
		Magazine = PartsActor;
		break;
	}

	GunInstance->EquipParts(PartsItem, PartsType);
}

void AGun::UnEquipParts(EPartsType PartsType)
{
	AParts* CurrentEquipParts = nullptr;
	switch (PartsType)
	{
	case EPartsType::Rail:
		CurrentEquipParts = Rail;
		Rail = nullptr;
		CurCameraSocketName = DefaultIronSight;
		ChangeCameraPos(CurCameraSocketName);
		break;
	case EPartsType::Muzzle:
		CurrentEquipParts = Muzzle;
		Muzzle = nullptr;
		break;
	case EPartsType::Magazine:
		CurrentEquipParts = Magazine;
		Magazine = nullptr;
		break;
	}

	if(CurrentEquipParts)
	{
		CurrentEquipParts->Destroy();
	}

	GunInstance->UnEquipParts(PartsType);
}

void AGun::SetGunState()
{
	//GunItemInstance를 토대로 현재 장착된 파츠 및 총알 등 모두 Update
	//(총이 교체되는 게임인경우)
	//이 게임은 총 교체는 불가능이라 구현 x
}

void AGun::SetGunAnimLayer()
{
	if(!IsValid(APC))
		return;
	
	UAnimInstance* AnimInstance = APC->GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->LinkAnimClassLayers(GunInstance->GunItemData->BP_GunAnimLayer);
	}
}

void AGun::OnRep_GunInstance()
{
	TryInit();
}

void AGun::OnRep_CurCameraSocketName()
{
	ChangeCameraPos(CurCameraSocketName);
}

void AGun::OnRep_FireSystemIndex()
{
	//ui업데이트 라이플일 경우에만
}

void AGun::OnRep_MagAmmo()
{
	if(IsValid(APC))
	{
		if(APC->IsLocallyControlled())
		{
			OnMagAmmoChanged.ExecuteIfBound(MagAmmo);
		}
	}
}

void AGun::OnRep_TotalAmmo()
{
	if(IsValid(APC))
	{
		if(APC->IsLocallyControlled())
		{
			OnTotalAmmoChanged.ExecuteIfBound(TotalAmmo);
		}
	}
}

void AGun::OnRep_bIsRealBullet()
{
	if(IsValid(APC))
	{
		if(APC->IsLocallyControlled())
		{
			OnMagAmmoTypeChanged.ExecuteIfBound(bIsRealBullet);
		}
	}
}

void AGun::ChangeCameraPos(FName SocketName)
{
	if(SightCamera)
	{
		SightCamera->AttachToComponent(
			SkeletalMeshComponent,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			SocketName);
	}
}

void AGun::BindCombatUI()
{
	Controller->GetUIManager()->GetCombatUI()->EquipGun();
	Controller->GetUIManager()->GetCombatUI()->BindTotalAmmoDelegate(OnTotalAmmoChanged);
	Controller->GetUIManager()->GetCombatUI()->BindMagAmmoDelegate(OnMagAmmoChanged);
	Controller->GetUIManager()->GetCombatUI()->BindMagAmmoTypeDelegate(OnMagAmmoTypeChanged);

	OnTotalAmmoChanged.ExecuteIfBound(TotalAmmo);
	OnMagAmmoChanged.ExecuteIfBound(MagAmmo);
	OnMagAmmoTypeChanged.ExecuteIfBound(bIsRealBullet);
}

void AGun::OnMagItemAdd(FName ItemId, int Amount)
{
	if(!IsValid(GunInstance))
		return;

	if(GunInstance->GunItemData->BulletItemData->GetItemId().IsEqual(ItemId))
	{
		TotalAmmo += Amount;
		OnTotalAmmoChanged.ExecuteIfBound(TotalAmmo);
	}
}






///VFX
void AGun::PlayShotEffect() const
{
	if(IsValid(APC))
	{
		//몽타주 실행
		APC->PlayAnimMontage(GunInstance->GunItemData->ShotMontage);

		FName MuzzleFlashSocketName = TEXT("Flash");
		
		UPoolManager* PoolManager = GetWorld()->GetSubsystem<UPoolManager>();
		USceneComponent* MeshComponent = SkeletalMeshComponent;
		if(IsValid(Muzzle))
		{
			MeshComponent = Muzzle->GetPartsMesh();
		}
		
		PoolManager->Client_PlayAttachedFX(
			GunInstance->GunItemData->MuzzleFlashFX,
			MeshComponent,
			MuzzleFlashSocketName);
	}
}

void AGun::Multicast_PlayShotEffect_Implementation() const
{
	if(HasAuthority())
		return;

	PlayShotEffect();
}


void AGun::Server_ShowHitScanChannel(FHitScanInfo& HitScanInfo)
{
	ShowHitScanChannel(HitScanInfo);

	//multicast
	Multicast_ShowHitScanChannel(HitScanInfo);
}

void AGun::Multicast_ShowHitScanChannel_Implementation(FHitScanInfo HitScanInfo)
{
	if(HasAuthority())
		return;
	
	ShowHitScanChannel(HitScanInfo);
}

void AGun::ShowHitScanChannel(FHitScanInfo& HitScanInfo)
{
	UPoolManager* PoolManager = GetWorld()->GetSubsystem<UPoolManager>();
	
	UNiagaraComponent* Comp = PoolManager->Client_PlayLocationFX(
		HitScanInfo.HitScanEffect,
		HitScanInfo.StartLoc,
		FRotator::ZeroRotator);

	Comp->SetVectorParameter(HitScanInfo.StartName, HitScanInfo.StartLoc);
	Comp->SetVectorParameter(HitScanInfo.EndName, HitScanInfo.EndLoc);
}


void AGun::Server_SpawnHitImpact(FHitScanImpactInfo& HitScanImpactInfo)
{
	SpawnHitImpact(HitScanImpactInfo);
	Multicast_SpawnHitImpact(HitScanImpactInfo);
}

void AGun::Multicast_SpawnHitImpact_Implementation(FHitScanImpactInfo HitScanImpactInfo)
{
	if(HasAuthority())
		return;

	SpawnHitImpact(HitScanImpactInfo);
}

void AGun::SpawnHitImpact(FHitScanImpactInfo& HitScanImpactInfo)
{
	//맞은 곳에 임팩트 소환
	UImpactManager* ImpactManager = GetWorld()->GetSubsystem<UImpactManager>();
	ImpactManager->SpawnImpact(
		HitScanImpactInfo.Surface,
		HitScanImpactInfo.HitLoc,
		FRotator::ZeroRotator);

	//맞은곳에 데칼 소환
	UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		HitScanImpactInfo.DecalMaterial,
		HitScanImpactInfo.DecalSize,
		HitScanImpactInfo.HitLoc,
		HitScanImpactInfo.DecalRot,
		HitScanImpactInfo.DecalLifeSpan
		)->SetFadeScreenSize(0.0015f);
}




///Sound
void AGun::PlayFireSound()
{
	if(IsValid(APC) && IsValid(SoundManager))
	{
		if(APC->IsLocallyControlled())
		{
			SoundManager->PlaySound2D(
				GunInstance->GunItemData->ShotSFX);
		}
		else
		{
			SoundManager->PlaySoundAtLocation(
				GunInstance->GunItemData->ShotSFX, GetActorLocation());
		}
	}
}

void AGun::Multicast_PlayFireSound_Implementation()
{
	if(HasAuthority())
		return;

	PlayFireSound();
}

void AGun::PlayEquipGunSound()
{
	if(IsValid(APC) && IsValid(SoundManager))
	{
		if(APC->IsLocallyControlled())
		{
			SoundManager->PlaySound2D(GunInstance->GunItemData->EquipSFX);
		}
		else
		{
			SoundManager->PlaySoundAtLocation(GunInstance->GunItemData->EquipSFX, GetActorLocation());
		}
	}
}