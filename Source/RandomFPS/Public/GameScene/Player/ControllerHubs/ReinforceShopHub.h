// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "ReinforceShopHub.generated.h"


class APlayerCharacter;
class AReinforceShop;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UReinforceShopHub : public UActorComponent
{
	GENERATED_BODY()

public:	
	UReinforceShopHub();

	UFUNCTION(Client, Reliable)
	void Client_OpenReinforceUI(AReinforceShop* RS, APlayerCharacter* APC);
	UFUNCTION(Client, Reliable)
	void Client_FailToReinforce(AReinforceShop* RS);
	UFUNCTION(Client, Reliable)
	void Client_SuccessToAwake(AReinforceShop* RS, int CurGunLevel, int RemainMoney);
	UFUNCTION(Client, Reliable)
	void Client_SuccessToReinforce(AReinforceShop* RS, int CurGunLevel, int RemainMoney);
	UFUNCTION(Client,Reliable)
	void Client_ReinforceButFailByProb(AReinforceShop* RS, int RemainMoney);

	
	
	UFUNCTION(Server,Reliable)
	void Server_ReinforceGun(AReinforceShop* RS, APlayerCharacter* APC);
	UFUNCTION(Server,Reliable)
	void Server_CloseShopUI(AReinforceShop* RS, APlayerCharacter*APC);

	
protected:
	virtual void BeginPlay() override;

		
};
