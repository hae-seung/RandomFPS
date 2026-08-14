// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameScene/InteractableObject/InteractableUIObject.h"
#include "ReinforceShop.generated.h"

class UReinforceShopHub;
class UReinforceShopUI;

UCLASS()
class RANDOMFPS_API AReinforceShop : public AInteractableUIObject
{
	GENERATED_BODY()
	
public:	
	virtual void OpenUI(APlayerCharacter* APC) override;
	virtual void Interact(APlayerCharacter* APC) override;

	//client
	void RequestReinforce();
	void FailToReinforce();
	void FailByProb(int RemainMoney);
	void SuccessAwakeGun(int CurGunLevel, int RemainMoney);
	void SuccessToReinforce(int CurGunLevel, int RemainMoney);
	void CloseUI();

	//server
	void ConfirmReinforce(APlayerCharacter* APC, UReinforceShopHub* ShopHub);

	
	
	
protected:
	virtual void BeginPlay() override;
	AReinforceShop();

	
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess))
	UBoxComponent* BoxCollision;


private:
	bool bIsInit;
	UPROPERTY()
	APlayerCharacter* Client_APC;
	UPROPERTY()
	UReinforceShopHub* ClientHub;
	
	UPROPERTY()
	UReinforceShopUI* ReinforceShopUI;
};
