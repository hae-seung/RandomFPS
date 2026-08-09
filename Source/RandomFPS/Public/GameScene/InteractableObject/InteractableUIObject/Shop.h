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
	virtual void Interact(APlayerCharacter* APC, UPlayerInteractSystem* InteractSystem) override;
	virtual void OpenUI(APlayerCharacter* APC) override;
	virtual void StopInteract() override;

	
	void CloseShopUI();
	FShopState& GetClientShopState();
	
	//client
	void RequestBuyItem(const FShopBuyContext& BuyContext);
	void FailToBuy();
	void BuySuccess(const FShopBuyContextFeedback& BuyContextFeedback);

	//server
	void BuyItemConfirm(const FShopBuyContext& BuyContext, APlayerCharacter* APC, UShopHub* ShopHub);

	
protected:
	AShop();


	
private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere)
	float CameraBlendTime;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere)
	UShopData* ShopData;

private:
	
	bool bIsInit;
	const FShopBuyContext* LastClientRequestBuyContext;
	
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
	
	//클라 동기화용 캐싱
	/*FShopState 내부에 UObject를 사용하므로 UPROPERTY로 엔진에 등록하여
	참조 연결고리를 만들어주어야 한다.*/
	UPROPERTY()
	FShopState ClientShop;
};
