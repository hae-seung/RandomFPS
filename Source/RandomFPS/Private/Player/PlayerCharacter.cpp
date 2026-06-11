// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameScene/Player/MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameScene/Player/Components/CardManager.h"
#include "GameScene/Player/Components/Inventory.h"
#include "GameScene/Player/Components/PlayerCombatSystem.h"
#include "GameScene/Player/Components/PlayerWeapon.h"
#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemData/PartsData/RailPartsData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "UI/UIManager.h"


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
///
///										Init
///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#pragma region Init

APlayerCharacter::APlayerCharacter()
{
	SetGenericTeamId(FGenericTeamId(0));
	
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MakeComponents();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(!IsLocallyControlled()) return;

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(AMyPlayerController* LocalController = Cast<AMyPlayerController>(GetController()))
		{
			MyController = LocalController;
			if(MyController->PlayerCameraManager)
			{
				MyController->PlayerCameraManager->ViewPitchMin = -80.f;
				MyController->PlayerCameraManager->ViewPitchMax = 55.f;
			}
			BindKey(EnhancedInputComponent);
		}
	}
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Inventory->SetComponents(PlayerWeapon);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("beginCharacter"));
	bIsSprint = false;
	bIsAiming = false;
	bIsThirdPerspective = true;

	CharacterMovementComp = GetCharacterMovement();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(Controller)
	{
		AimPitch = Controller->GetControlRotation().Pitch;
		if (AimPitch > 180.f)
		{
			AimPitch -= 360.f;
		}
		AimPitch = FMath::Clamp(AimPitch, -75.f, 55.f);
		
		Server_ChangeAimPitch(AimPitch);
	}

	GetLeftHandIKTransform();
}

bool APlayerCharacter::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWrote = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if(Inventory)
	{
		bWrote |= Inventory->ReplicateSubobjects(Channel, Bunch, RepFlags);
	}

	return bWrote;
}

void APlayerCharacter::MakeComponents()
{
	CardManager = CreateDefaultSubobject<UCardManager>("CardManager");
	PlayerWeapon = CreateDefaultSubobject<UPlayerWeapon>("PlayerWeapon");
	
	FP_Camera = CreateDefaultSubobject<UCameraComponent>("FP_Camera");
	FP_Camera->SetupAttachment(GetMesh(), TEXT("CameraSocket"));
	
	FP_Camera->SetAutoActivate(false);

	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));

	CombatSystem = CreateDefaultSubobject<UPlayerCombatSystem>(TEXT("CombatSystem"));
	CombatSystem->OnPlayerDead.AddUObject(this, &APlayerCharacter::Dead);
	CombatSystem->OnPlayerRevive.AddUObject(this, &APlayerCharacter::Revive);
}

void APlayerCharacter::Server_ChangeAimPitch_Implementation(float Pitch)
{
	if(!HasAuthority()) return;

	AimPitch = Pitch;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, AimPitch);
	DOREPLIFETIME(APlayerCharacter, bIsAiming);
	DOREPLIFETIME(APlayerCharacter, bIsDead);
}

void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if(!CharacterMovementComp || !IsLocallyControlled()) return;
	
	if(CharacterMovementComp->MovementMode == MOVE_Falling && bIsAiming)
	{
		bIsAiming = false;

		if(!bIsThirdPerspective)
		{
			bIsThirdPerspective = true;
			FollowCamera->SetActive(true);
			FP_Camera->SetActive(false);
		}

		MyController->SetViewTargetWithBlend(this, 0.15f);

		Server_ChangeZoomState(bIsAiming);
	}
}

#pragma endregion Init


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
///
///										Functions
///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#pragma region Functions

bool APlayerCharacter::HasWeapon() const
{
	if(PlayerWeapon && PlayerWeapon->bHasWeapon)
		return true;
	
	return false;
}

void APlayerCharacter::GetLeftHandIKTransform()
{
	if(!HasWeapon())
		return;
	
	// 1. Weapon Mesh에서 LeftHandIK 소켓 월드 위치/회전 가져오기
	FTransform LeftHandSocketWorld = PlayerWeapon->GetLeftHandIKTransform();
	
	// 2. Hand_R 본 기준으로 Bone Space로 변환
	FVector LocalLocation;
	FRotator LocalRotation;
	GetMesh()->TransformToBoneSpace(
		TEXT("hand_r"),                  // 기준 본
		LeftHandSocketWorld.GetLocation(), // 월드 위치
		LeftHandSocketWorld.GetRotation().Rotator(), // 월드 회전
		LocalLocation,                   // 출력 위치
		LocalRotation                    // 출력 회전
	);

	// 3. FTransform으로 합치기
	LeftHandIK = FTransform(LocalRotation, LocalLocation);
}

void APlayerCharacter::RequestAddItem(UItemData* ItemData, int Amount)
{
	if(HasAuthority())
	{
		UItemInstance* NewItem = ItemData->CreateItem(this);
		AddItem(NewItem, Amount);
	}
	else
	{
		Server_AddItem(ItemData, Amount);
	}
}

void APlayerCharacter::AddItem(UItemInstance* Item, int Amount)
{
	Inventory->AddItem(Item, Amount);
}

void APlayerCharacter::Server_AddItem_Implementation(UItemData* ItemData, int Amount)
{
	UItemInstance* NewItem = ItemData->CreateItem(this);
	AddItem(NewItem, Amount);
}

void APlayerCharacter::OnRep_bIsReloading()
{
	//리로딩이 끝난 알림은 따로 처리할게 없음.
	if(!bIsReloading)
		return;

	//리로딩이 시작되었다는 알림일때
	PlayReloadMontage();
}

void APlayerCharacter::PlayReloadMontage()
{
	if(UGunItem* GunItem = PlayerWeapon->GetCurrentEquipGun())
	{
		UAnimMontage* ReloadMontage = GunItem->GunItemData->ReloadMontage;
		
		if(GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage) > 0.f)
		{
			//서버일때만 종료 델리게이트 설정
			if(HasAuthority())
			{
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &APlayerCharacter::OnReloadMontageEnded);
				GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, ReloadMontage);
			}
		}
	}
}

EEntityType APlayerCharacter::GetEntityType()
{
	return EntityType;
}

void APlayerCharacter::TakeDamage(FDamageContext& Context) //override
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), Context.BaseDamage);
	CombatSystem->TakeDamage(Context);
}

//server
void APlayerCharacter::OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsReloading = false;
	
	if(bInterrupted)
	{
		//구현 필요없음
		return;
	}
	
	PlayerWeapon->Reload();
}

void APlayerCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	TeamId = TeamID;
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
	return TeamId;
}

//server
void APlayerCharacter::Dead()
{
	if(!HasAuthority())
		return;
	
	bIsDead = true; //replicated
	SetCharacterOptionDeadState();
}

void APlayerCharacter::OnRep_bIsDead()
{
	if(bIsDead)
	{
		SetCharacterOptionDeadState();
	}
	else
	{
		SetCharacterOptionAliveState();
	}
}

void APlayerCharacter::SetCharacterOptionDeadState()
{
	StopAnimMontage();
	
	bUseControllerRotationYaw = false;
	CharacterMovementComp->DisableMovement();
	
	if(IsLocallyControlled())
	{
		StopSprint();
		UnCrouch();
		ZoomQuick();
		ZoomHoldRelease();
		ShotRelease();
		if(!bIsThirdPerspective)
		{
			ChangePerspective();
		}
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Dead")); //되살아날땐 Pawn으로
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);//되살아날땐 ECR_Block으로
}

//server
void APlayerCharacter::Revive()
{
	if(!HasAuthority())
		return;
	
	bIsDead = false;
	SetCharacterOptionAliveState();
}

void APlayerCharacter::SetCharacterOptionAliveState()
{
	bUseControllerRotationYaw = true;
	CharacterMovementComp->SetMovementMode(MOVE_Walking);
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PawnCapsule")); //되살아날땐 Pawn으로
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); //되살아날땐 ECR_Block으로
}

bool APlayerCharacter::GetIsDead()
{
	return bIsDead;
}



#pragma endregion Functions


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
///
///										Input
///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#pragma region InputFunctions

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if(bIsDead)
		return;
	
	FVector2D MovementVector = Value.Get<FVector2d>();
	
	DoMove(MovementVector.X, MovementVector.Y);
}

void APlayerCharacter::DoMove(float Right, float Forward)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	
	if(Forward == 1.0f)
	{
		bCanSprint = true;
	}
	else
	{
		bCanSprint = false;
		StopSprint();
	}
	
	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	if(!IsLocallyControlled()) return;
	
	FVector2D LookAxisVector = Value.Get<FVector2d>();
	
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
	if(!IsLocallyControlled() || !Value.Get<bool>() || bIsAiming || bIsDead) return;
	Super::Jump();
}

void APlayerCharacter::Sprint(const FInputActionValue& Value)
{
	if(bIsSprint || !bCanSprint || !IsLocallyControlled() || bIsAiming || bIsReloading || bIsDead) return;
	
	bIsSprint = true;

	CharacterMovementComp->MaxWalkSpeed = SprintSpeed;
	
	if(HasAuthority())
	{
		MultiCast_ChangeSpeed(SprintSpeed);
	}
	else
	{
		Server_ChangeSpeed(SprintSpeed);
	}
}

void APlayerCharacter::StopSprint()
{
	if(!IsLocallyControlled() || !bIsSprint || bIsDead) return;
	
	bIsSprint = false;

	CharacterMovementComp->MaxWalkSpeed = WalkSpeed;
	
	if(HasAuthority())
	{
		MultiCast_ChangeSpeed(WalkSpeed);
	}
	else
	{
		Server_ChangeSpeed(WalkSpeed);
	}
	
}

void APlayerCharacter::Server_ChangeSpeed_Implementation(float Speed)
{
	MultiCast_ChangeSpeed(Speed);
}

void APlayerCharacter::MultiCast_ChangeSpeed_Implementation(float Speed)
{
	if(IsLocallyControlled()) return;
	
	CharacterMovementComp->MaxWalkSpeed = Speed;
}

void APlayerCharacter::ActionCrouch()
{
	if(!IsLocallyControlled() || bIsDead) return;
	
	if(CanCrouch())
	{
		Crouch();
	}
	else
		UnCrouch();
}

void APlayerCharacter::ZoomHold()
{
	if(!CanZoom() || !bIsThirdPerspective) return;
	
	bIsAiming  = true;
	
	if(!HasAuthority())
		Server_ChangeZoomState(bIsAiming);
	
	BP_StartZoom();
}

void APlayerCharacter::ZoomHoldRelease()
{
	if(!IsLocallyControlled()) return;
	
	bIsAiming  = false;

	if(!HasAuthority())
		Server_ChangeZoomState(bIsAiming);
	
	BP_StartZoomReverse();
}

void APlayerCharacter::ZoomQuick()
{
	if(bIsAiming)
	{
		//줌 종료
		bIsAiming  = false;
		MyController->SetViewTargetWithBlend(this, 0.15f);
	}
	else
	{
		if(!CanZoom()) return;
		
		bIsAiming  = true;
	}
	
	PlayerWeapon->ZoomShort(bIsAiming);
	
	if(!HasAuthority())
		Server_ChangeZoomState(bIsAiming);
}

bool APlayerCharacter::CanZoom() const
{
	if(bIsAiming || !IsLocallyControlled() || bIsSprint || bIsReloading || bIsDead)
		return false;

	if(GetCharacterMovement()->IsFalling())
		return false;
	
	return true;
}

void APlayerCharacter::Server_ChangeZoomState_Implementation(bool IsZoom)
{
	//replicated
	bIsAiming = IsZoom;
}

void APlayerCharacter::Shot()
{
	if(!PlayerWeapon || bIsReloading || bIsDead)
		return;

	//총을 쐇다 라는 신호만 주면됨.
	//나머지는 총 발사 모드에서 타이머로 알아서 처리 할거임.
	PlayerWeapon->Server_Shot();
}

void APlayerCharacter::ShotRelease()
{
	if(!PlayerWeapon)
		return;
	
	PlayerWeapon->Server_StopShot();
}

void APlayerCharacter::ChangePerspective()
{
	if(!IsLocallyControlled()) return;
	
	if(bIsThirdPerspective)
	{
		//1인칭으로 변경
		if(bIsDead)
			return;
		
		FollowCamera->SetActive(false);
		FP_Camera->SetActive(true);
	}
	else
	{
		//3인칭으로 변경
		FollowCamera->SetActive(true);
		FP_Camera->SetActive(false);
	}

	bIsThirdPerspective = !bIsThirdPerspective;
}

void APlayerCharacter::Reload()
{
	Server_TryReload();
}

void APlayerCharacter::Server_TryReload_Implementation()
{
	if(!CanReload())
		return;

	bIsReloading = true;
	PlayReloadMontage();
}

bool APlayerCharacter::CanReload()
{
	if(bIsAiming || bIsSprint || bIsReloading || !HasWeapon() ||
		!PlayerWeapon->CanReload() || bIsDead)
		return false;
	
	return true;
}

void APlayerCharacter::ToggleInventory()
{
	if(!IsLocallyControlled()) return;
	if(!MyController->GetUIManager()) return;

	MyController->GetUIManager()->ToggleInventory();
}


void APlayerCharacter::BindKey(UEnhancedInputComponent* EIC)
{
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	
	EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
	
	EIC->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
	EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
	
	EIC->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::ActionCrouch);

	EIC->BindAction(ZoomQuickAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ZoomQuick);
	
	EIC->BindAction(ZoomHoldAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ZoomHold);
	EIC->BindAction(ZoomHoldAction, ETriggerEvent::Completed, this, &APlayerCharacter::ZoomHoldRelease);

	EIC->BindAction(ShotAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Shot);
	EIC->BindAction(ShotAction, ETriggerEvent::Completed, this, &APlayerCharacter::ShotRelease);

	EIC->BindAction(PerspectiveAction, ETriggerEvent::Started, this, &APlayerCharacter::ChangePerspective);

	EIC->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::Reload);
	
	EIC->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleInventory);
}

#pragma endregion InputFunctions