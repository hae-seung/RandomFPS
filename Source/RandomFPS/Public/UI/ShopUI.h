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
	void FailToBuy();
	void BuySuccess(const FShopBuyContextFeedback& BuyContextFeedback);
	
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
	UPROPERTY(meta=(BindWidget))
	UShopBuyTabUI* BuyTabUI;
	UPROPERTY(meta=(BindWidget))
	UShopSellTabUI* SellTabUI;
	UPROPERTY(meta=(BindWidget))
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
