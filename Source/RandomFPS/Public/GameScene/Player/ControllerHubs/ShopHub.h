// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "Struct/ShopStruct.h"
#include "ShopHub.generated.h"


class APlayerCharacter;
class AShop;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UShopHub : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShopHub();

	UFUNCTION(Client, Reliable)
	void Client_OpenWorldUI(AShop* ShopActor, APlayerCharacter* APC);
	UFUNCTION(Client, Reliable)
	void Client_BuyError(AShop* ShopActor);
	UFUNCTION(Client, Reliable)
	void Client_BuySuccess(AShop* ShopActor, FShopBuyContextFeedback BuyContextFeedback);
	UFUNCTION(Client, Reliable)
	void Client_SellError(AShop* ShopActor);
	UFUNCTION(Client, Reliable)
	void Client_SellSuccess(AShop* ShopActor, FShopSellContextFeedback SellContextFeedback);
	
	UFUNCTION(Server, Reliable)
	void Server_BuyItem(FShopBuyContext BuyContext, AShop* ShopActor, APlayerCharacter* APC);
	UFUNCTION(Server, Reliable)
	void Server_CloseShopUI(AShop* ShopActor, APlayerCharacter* APC);
	UFUNCTION(Server, Reliable)
	void Server_SellItem(AShop* ShopActor, APlayerCharacter* APC, FShopSellContext SellContext);

	
protected:
	virtual void BeginPlay() override;

};
