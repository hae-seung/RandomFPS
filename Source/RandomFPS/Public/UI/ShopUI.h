// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldInteractionUI.h"
#include "Struct/ShopStruct.h"
#include "ShopUI.generated.h"

class UTextBlock;
class APlayerCharacter;
class UBuyQuantityTab;
class AShop;
class UShopSellTabUI;
class UShopBuyTabUI;
class UButton;



UCLASS()
class RANDOMFPS_API UShopUI : public UWorldInteractionUI
{
	GENERATED_BODY()

public:
	virtual void Open() override;
	void FirstOpenInit(AShop* ShopObject, APlayerCharacter* APC);
	void OpenBuyQuantityTab(const FShopItemEntry* ShopItemEntry, EShopCategoryType InItemCategory);

	//구매탭에서는 싱크가 맞기전까지 구매확정 버튼만 못누름. 나머지는 계속 가능함
	void FailToBuy();
	void BuySuccess(const FShopBuyContextFeedback& BuyContextFeedback);
	//판매탭에서는 싱크가 맞기전까지 판매확정 버튼 및 다른 판매가능 품목에 접근도 못함.
	void FailToSell();
	void SellSuccess(const FShopSellContextFeedback& SellContextFeedback);
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* UserMoneyText;
	UPROPERTY(meta=(BindWidget))
	UButton* BuyBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* SellBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* CloseBtn;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget, AllowPrivateAccess))
	UShopBuyTabUI* BuyTabUI;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget, AllowPrivateAccess))
	UShopSellTabUI* SellTabUI;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget, AllowPrivateAccess))
	UBuyQuantityTab* BuyQuantityTab;


private:
	UPROPERTY()
	AShop* ShopActor;
	
private:
	UFUNCTION()
	void OpenBuyTab();
	UFUNCTION()
	void OpenSellTab();
	UFUNCTION()
	void CloseShopUI();
};
