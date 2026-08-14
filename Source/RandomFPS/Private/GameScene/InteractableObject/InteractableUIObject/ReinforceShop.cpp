// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/InteractableUIObject/ReinforceShop.h"

#include "Components/BoxComponent.h"
#include "GameScene/Cards/StructHeader.h"
#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerWalletSystem.h"
#include "GameScene/Player/Components/PlayerWeapon.h"
#include "GameScene/Player/ControllerHubs/ReinforceShopHub.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "UI/ReinforceShopUI.h"


AReinforceShop::AReinforceShop()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision= CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

void AReinforceShop::BeginPlay()
{
	Super::BeginPlay();

	ReinforceShopUI = Cast<UReinforceShopUI>(WorldInteractionUI);
}

void AReinforceShop::Interact(APlayerCharacter* APC)
{
	AMyPlayerController* Controller = Cast<AMyPlayerController>(APC->GetController());
	if(!Controller)
	{
		StopInteract(APC);
		return;
	}

	if(UReinforceShopHub* Hub = Controller->GetReinforceShopHub())
	{
		Hub->Client_OpenReinforceUI(this, APC);
	}
	
	APC->ToggleCharacterMoveState(false);
}

void AReinforceShop::OpenUI(APlayerCharacter* APC)
{
	if(!bIsInit)
	{
		Client_APC = APC;
		ReinforceShopUI->FirstOpenInit(this, APC);

		if(const AMyPlayerController* AMC =
			Cast<AMyPlayerController>(APC->GetController()))
		{
			ClientHub = AMC->GetReinforceShopHub();
		}
		
		bIsInit = true;
	}

	Super::OpenUI(APC);
	
	APC->ToggleCharacterMoveState(false);
	APC->ChangeCameraViewTarget(this, CameraBlendTime);
	APC->ChangeWidgetInteraction(CameraComponent);
}

void AReinforceShop::CloseUI()
{
	Client_APC->ToggleCharacterMoveState(true);
	Client_APC->ChangeCameraViewTarget(Client_APC, CameraBlendTime);
	Client_APC->ChangeWidgetInteraction();

	ClientHub->Server_CloseShopUI(this, Client_APC);
}



//client -> server
void AReinforceShop::RequestReinforce()
{
	ClientHub->Server_ReinforceGun(this, Client_APC);
}

//server
void AReinforceShop::ConfirmReinforce(APlayerCharacter* APC, UReinforceShopHub* ShopHub)
{
	//강화 시작 후 실패든 성공이든 허브를 통해 클라에게 전달
	//성공시에는 성공한 직후의 그 레벨을 보냄
	UPlayerWalletSystem* Wallet = APC->GetWalletSystem();
	if(!Wallet)
	{
		ShopHub->Client_FailToReinforce(this);
		UE_LOG(LogTemp,Warning, TEXT("No Wallet"));
		return;
	}
	
	UGunItem* Gun = APC->GetWeaponSystem()->GetCurrentEquipGun();
	if(!Gun)
	{
		ShopHub->Client_FailToReinforce(this);
		UE_LOG(LogTemp,Warning, TEXT("No Gun"));
		return;
	}
	const FGunAbilityWrapper* Wrapper = Gun->GetAbilityWrapper(Gun->GetGunLevel());
	if(!Wrapper)
	{
		ShopHub->Client_FailToReinforce(this);
		UE_LOG(LogTemp,Warning, TEXT("No Wrapper"));
		return;
	}
	if(Wrapper->NextUpgradeNeedMoney > Wallet->GetMoney())
	{
		ShopHub->Client_FailToReinforce(this);
		UE_LOG(LogTemp,Warning, TEXT("No Enough Money"));
		return;
	}

	//실제 강화 시도
	Wallet->SpendMoney(Wrapper->NextUpgradeNeedMoney);

	const int RandNum = FMath::RandRange(1, 100);
	if(RandNum <= Wrapper->NextUpgradeProb)
	{
		Gun->LevelUp();
		if(Gun->IsAwake())
		{
			ShopHub->Client_SuccessToAwake(this, Gun->GetGunLevel(), Wallet->GetMoney());
			UE_LOG(LogTemp,Warning, TEXT("Yes Awake"));
		}
		else
		{
			ShopHub->Client_SuccessToReinforce(this, Gun->GetGunLevel(), Wallet->GetMoney());
			UE_LOG(LogTemp,Warning, TEXT("Yes Reinforce"));
		}
	}
	else
	{
		ShopHub->Client_ReinforceButFailByProb(this, Wallet->GetMoney());
	}
}

void AReinforceShop::FailToReinforce()
{
	ReinforceShopUI->FailToReinforce();
}

void AReinforceShop::FailByProb(int RemainMoney)
{
	ReinforceShopUI->FailToReinforce(RemainMoney);
}


void AReinforceShop::SuccessAwakeGun(int CurGunLevel, int RemainMoney)
{
	ReinforceShopUI->SuccessAwakeGun(CurGunLevel, RemainMoney);
}

void AReinforceShop::SuccessToReinforce(int CurGunLevel, int RemainMoney)
{
	ReinforceShopUI->SuccessReinforceGun(CurGunLevel, RemainMoney);
}
