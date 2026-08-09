// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/InteractableObject/InteractableUIObject/Shop.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/Inventory.h"
#include "GameScene/Player/Components/PlayerInteractSystem.h"
#include "GameScene/Player/Components/PlayerWalletSystem.h"
#include "GameScene/Player/ControllerHubs/ShopHub.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "GameScene/Player/ItemData/ShopData.h"
#include "UI/ShopUI.h"

AShop::AShop()
{
	bReplicates = true;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(StaticMeshComp);
}

void AShop::BeginPlay()
{
	Super::BeginPlay();

	ShopUI = Cast<UShopUI>(WorldInteractionUI);
}

//server
void AShop::Interact(APlayerCharacter* APC, UPlayerInteractSystem* InteractSystem)
{
	Super::Interact(APC, InteractSystem);
	
	AController* Controller = APC->GetController();
	
	if(!PlayerShops.Find(APC))
	{
		PlayerShops.Add(APC, ShopData->GetShopData());
	}
	
	if(const AMyPlayerController* AMC = Cast<AMyPlayerController>(Controller))
	{
		if(UShopHub* ShopHub = AMC->GetShopHub())
		{
			
			ShopHub->Client_OpenWorldUI(this, APC);
		}
	}
	
	APC->ToggleCharacterMoveState(false);
}

//server
void AShop::StopInteract()
{
	Super::StopInteract();
	Server_APC->ToggleCharacterMoveState(true);
}


//client
void AShop::OpenUI(APlayerCharacter* APC)
{
	if(!bIsInit)
	{
		//클라전용을 위해 데이터로부터 복사
		ClientShop = ShopData->GetShopData();
		ShopUI->FirstOpenInit(this, APC);

		//클라이언트 월드에서 각자 상점 통신채널인 ShopHub를 캐싱
		if(const AMyPlayerController* AMC =
			Cast<AMyPlayerController>(APC->GetController()))
		{
			ClientShopHub = AMC->GetShopHub();
		}
		
		Client_APC = APC;
		bIsInit = true;
	}
	
	Super::OpenUI(APC);
	
	APC->ToggleCharacterMoveState(false);
	APC->ChangeCameraViewTarget(this, CameraBlendTime);
	APC->ChangeWidgetInteraction(CameraComponent);
}

void AShop::CloseShopUI()
{
	Client_APC->ToggleCharacterMoveState(true);
	Client_APC->ChangeCameraViewTarget(Client_APC, CameraBlendTime);
	Client_APC->ChangeWidgetInteraction();

	ClientShopHub->Server_CloseShopUI(this);
}


FShopState& AShop::GetClientShopState()
{
	return ClientShop;
}

//client -> server
void AShop::RequestBuyItem(const FShopBuyContext& BuyContext)
{
	UE_LOG(LogTemp,Warning,TEXT("Client Request Buy Item"));
	ClientShopHub->Server_BuyItem(BuyContext, this, Client_APC);
}

//server
void AShop::BuyItemConfirm(const FShopBuyContext& BuyContext, APlayerCharacter* APC, UShopHub* ShopHub)
{
	FShopState* ShopState = PlayerShops.Find(APC);

	if(!APC)
	{
		UE_LOG(LogTemp,Warning,TEXT("No APC"));
	}
	
	if(!ShopState)
	{
		UE_LOG(LogTemp,Warning,TEXT("No ShopState"));
		ShopHub->Client_BuyError(this);
		return;
	}

	//서버에서 검증 후 클라로 다시 보냄.
	FShopCategory* ShopCategory = ShopState->Categories.FindByPredicate(
		[&BuyContext](const FShopCategory& Category)
		{
			return Category.CategoryType == BuyContext.ItemType;
		});

	if(!ShopCategory)
	{
		UE_LOG(LogTemp,Warning,TEXT("No ShopCategory"));
		ShopHub->Client_BuyError(this);
		return;
	}

	FShopItemEntry* ShopItem = ShopCategory->Items.FindByPredicate(
		[&BuyContext](const FShopItemEntry& ItemEntry)
		{
			return ItemEntry.ItemData->GetItemId() == BuyContext.ItemID;
		});

	if(!ShopItem)
	{
		UE_LOG(LogTemp,Warning,TEXT("No ShopItem"));
		ShopHub->Client_BuyError(this);
		return;
	}
	
	if(BuyContext.BuyAmount <= 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("수량 틀림"));
		ShopHub->Client_BuyError(this);
		return;
	}

	if(ShopItem->Stock != -1 && ShopItem->Stock < BuyContext.BuyAmount)
	{
		UE_LOG(LogTemp,Warning,TEXT("재고 틀림"));
		ShopHub->Client_BuyError(this);
		return;
	}
	
	const int CurMoney = APC->GetWalletSystem()->GetMoney();
	const int SpendMoney = BuyContext.BuyAmount * ShopItem->Price;

	if(CurMoney < SpendMoney)
	{
		UE_LOG(LogTemp,Warning,TEXT("가격 틀림"));
		ShopHub->Client_BuyError(this);
		return;
	}
	
	//인벤토리 지급
	const int Remain =
		APC->GetInventory()->AddItem(ShopItem->ItemData->CreateItem(APC), BuyContext.BuyAmount);

	//인벤토리 공간이 딸려서 원하는 물건의 갯수만큼 못샀을 수 있음
	const int RealBuyAmount = BuyContext.BuyAmount - Remain;
	
	APC->GetWalletSystem()->SpendMoney(RealBuyAmount * ShopItem->Price);
	if(ShopItem->Stock != -1)
	{
		ShopItem->Stock -= RealBuyAmount;
	}
	
	//클라에 구매 성공 피드백
	ShopHub->Client_BuySuccess(this,FShopBuyContextFeedback(
			BuyContext.ItemType,
			BuyContext.ItemID,
			RealBuyAmount,
			APC->GetWalletSystem()->GetMoney()));
}

//client
void AShop::FailToBuy()
{
	ShopUI->FailToBuy();
}

//client
void AShop::BuySuccess(const FShopBuyContextFeedback& BuyContextFeedback)
{
	ShopUI->BuySuccess(BuyContextFeedback);
}
