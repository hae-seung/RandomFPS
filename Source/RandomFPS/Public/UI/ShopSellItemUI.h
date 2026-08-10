// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/EnumHeader/FInventoryItem.h"
#include "ShopSellItemUI.generated.h"

class UTextBlock;
class UImage;
class UButton;

//아이템, 인벤토리인덱스, UIIndex, 개당가격
DECLARE_DELEGATE_FourParams(FOnClickedSellItemUI, const UItemInstance*, int, int, int);


UCLASS()
class RANDOMFPS_API UShopSellItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnClickedSellItemUI OnClickedSellItemUI;

	
public:
	void SetUIIndex(int Index);
	void SetItemInfoAndOpen(const FInventoryItem& Item, int PerPrice);
	void Close();
	void SetCanClickState(bool bState);
	void SetRemainStock(int RemainAmount);

protected:
	virtual void NativeOnInitialized() override;
	

private:
	UPROPERTY(meta=(BindWidget))
	UButton* SellItemBtn;
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StockText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PerPriceText;

private:
	//변동값
	UPROPERTY()
	const UItemInstance* ItemInstance;
	int ItemInventoryIndex;
	int ItemPrice;
	
	//고정값
	int SlotIndex;

private:
	UFUNCTION()
	void SellItemClicked();
};
