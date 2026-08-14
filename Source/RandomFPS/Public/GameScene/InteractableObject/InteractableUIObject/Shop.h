// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameScene/InteractableObject/InteractableUIObject.h"
#include "Struct/ShopStruct.h"
#include "Shop.generated.h"


class UShopHub;
class AMyPlayerController;
class UShopUI;
class UShopData;
class UPlayerInteractSystem;

UCLASS()
class RANDOMFPS_API AShop : public AInteractableUIObject
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Interact(APlayerCharacter* APC) override;
	virtual void OpenUI(APlayerCharacter* APC) override;

	
	void CloseShopUI();
	FShopState& GetClientShopState();
	const TMap<FName, FShopSellEntry>& GetSellableItemMap();
	
	//client buy
	void RequestBuyItem(const FShopBuyContext& BuyContext);
	void FailToBuy();
	void BuySuccess(const FShopBuyContextFeedback& BuyContextFeedback);
	//client sell
	const TArray<FShopSellEntry>& GetShopSellData() const;
	void RequestSellItem(const FShopSellContext& SellContext);
	void FailToSell();
	void SellSuccess(const FShopSellContextFeedback& SellContextFeedback);
	
	//server
	void BuyItemConfirm(const FShopBuyContext& BuyContext, APlayerCharacter* APC, UShopHub* ShopHub);
	void SellItemConfirm(const FShopSellContext& SellContext, APlayerCharacter* APC, UShopHub* ShopHub);
	
protected:
	AShop();


	
private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere)
	UShopData* ShopData;

private:
	bool bIsServerInit;
	bool bIsClientInit;
	
	UPROPERTY()
	UShopUI* ShopUI;
	
	//클라이언트에서 서버로 가기 위한 통신 채널
	UPROPERTY()
	UShopHub* ClientShopHub;
	UPROPERTY()
	APlayerCharacter* Client_APC;

	
	//서버관리용
	UPROPERTY()
	TMap<APlayerCharacter*, FShopState> PlayerShops;
	//서버 및 클라 모두 보유
	UPROPERTY()
	TMap<FName, FShopSellEntry> SellableItems;
	
	//클라이언트 개인 상점 => 서버에서 관리중인 개인상점과 동기화용
	/*FShopState 내부에 UObject를 사용하므로 UPROPERTY로 엔진에 등록하여
	참조 연결고리를 만들어주어야 한다.*/
	UPROPERTY()
	FShopState ClientShop;

private:
	void InitSellEntryData();
};
