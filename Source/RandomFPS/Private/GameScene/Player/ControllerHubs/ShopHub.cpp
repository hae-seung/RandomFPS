// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ControllerHubs/ShopHub.h"

#include "GameScene/InteractableObject/InteractableUIObject/Shop.h"

UShopHub::UShopHub()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UShopHub::Server_CloseShopUI_Implementation(AShop* ShopActor, APlayerCharacter* APC)
{
	ShopActor->StopInteract(APC);
}

void UShopHub::BeginPlay()
{
	Super::BeginPlay();
}

void UShopHub::Client_OpenWorldUI_Implementation(AShop* ShopActor, APlayerCharacter* APC)
{
	ShopActor->OpenUI(APC);
}

void UShopHub::Client_BuyError_Implementation(AShop* ShopActor)
{
	ShopActor->FailToBuy();
}

void UShopHub::Client_BuySuccess_Implementation(AShop* ShopActor, FShopBuyContextFeedback BuyContextFeedback)
{
	UE_LOG(LogTemp,Warning,TEXT("Clinet BuySuccess 수신"));
	ShopActor->BuySuccess(BuyContextFeedback);
}


void UShopHub::Server_BuyItem_Implementation(FShopBuyContext BuyContext, AShop* ShopActor,  APlayerCharacter* APC)
{
	UE_LOG(LogTemp,Warning, TEXT("Server BuyItem 수신"));
	ShopActor->BuyItemConfirm(BuyContext, APC, this);
}