// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/ShopStruct.h"
#include "BuyQuantityTab.generated.h"

class UPlayerWalletSystem;
class UAmountDeltaButton;
class UButton;
class UTextBlock;
class UImage;
class UShopUI;


DECLARE_DELEGATE_OneParam(FOnBuyConfirm, const FShopBuyContext&);


UCLASS()
class RANDOMFPS_API UBuyQuantityTab : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnBuyConfirm OnBuyConfirm;

	
public:
	void Open();
	UFUNCTION()
	void Close();
	void InitFirstOpen(UPlayerWalletSystem* InWalletSystem);
	void SetQuantityTab(const FShopItemEntry* EntryItem, EShopCategoryType InItemCategory);
	void SetSyncState(bool bState);

protected:
	virtual void NativeOnInitialized() override;
	

private:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StockText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PriceText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemAmountSelectText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* FinalPriceText;
	UPROPERTY(meta=(BindWidget))
	UButton* BuyBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* CancelBtn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* AmountMinus10Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* AmountMinus5Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* AmountMinus1Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* AmountPlus1Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* AmountPlus5Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* AmountPlus10Btn;

private:
	const FShopItemEntry* CurItem;
	int CurSelectAmount = 0;
	int TotalPrice = 0;
	bool bIsSyncComplete;
	
	EShopCategoryType ItemCategory;
	
	UPROPERTY()
	UPlayerWalletSystem* PlayerWallet;
	
private:
	void ChangeSelectAmount(int Delta);
	UFUNCTION()
	void ConfirmBuy();
};
