// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/ControllerHubs/ReinforceShopHub.h"

#include "GameScene/InteractableObject/InteractableUIObject/ReinforceShop.h"

UReinforceShopHub::UReinforceShopHub()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}


void UReinforceShopHub::BeginPlay()
{
	Super::BeginPlay();
	
}

void UReinforceShopHub::Client_OpenReinforceUI_Implementation(AReinforceShop* RS, APlayerCharacter* APC)
{
	RS->OpenUI(APC);
}

void UReinforceShopHub::Server_CloseShopUI_Implementation(AReinforceShop* RS, APlayerCharacter* APC)
{
	RS->StopInteract(APC);
}


void UReinforceShopHub::Server_ReinforceGun_Implementation(AReinforceShop* RS, APlayerCharacter* APC)
{
	RS->ConfirmReinforce(APC, this);
}

void UReinforceShopHub::Client_FailToReinforce_Implementation(AReinforceShop* RS)
{
	RS->FailToReinforce();
}

void UReinforceShopHub::Client_ReinforceButFailByProb_Implementation(AReinforceShop* RS, int RemainMoney)
{
	RS->FailByProb(RemainMoney);
}


void UReinforceShopHub::Client_SuccessToAwake_Implementation(AReinforceShop* RS, int CurGunLevel, int RemainMoney)
{
	RS->SuccessAwakeGun(CurGunLevel, RemainMoney);
}

void UReinforceShopHub::Client_SuccessToReinforce_Implementation(AReinforceShop* RS, int CurGunLevel, int RemainMoney)
{
	RS->SuccessToReinforce(CurGunLevel, RemainMoney);
}
