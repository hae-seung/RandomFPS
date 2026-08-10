// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/ShopStruct.h"
#include "ShopSellTabUI.generated.h"

class UItemInstance;
class UWrapBox;
class UShopSellItemUI;
class UInventory;
class UAmountDeltaButton;
class UButton;
class UImage;
class UTextBlock;
class UOverlay;

DECLARE_DELEGATE_OneParam(FOnSellConfirm, const FShopSellContext&);


UCLASS()
class RANDOMFPS_API UShopSellTabUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnSellConfirm OnSellConfirm;
	
	
public:
	void Open();
	void Close();
	void FirstOpenInit(UInventory* Inventory, const TMap<FName, FShopSellEntry>& SellEntryMap);
	void SetSyncState(bool bState);
	void UpdateSellTabByFeedback(const FShopSellContextFeedback& SellContextFeedback);
	
protected:
	virtual void NativeOnInitialized() override;

private:
#pragma region BindWidget
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryItemBox;
	
	UPROPERTY(meta=(BindWidget))
	UOverlay* SellQuantityTab;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameText;
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StockText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PerPriceText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SelectSellAmountText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* FinalPriceText;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* MinBtn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* Minus10Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* Minus1Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* Plus1Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* Plus10Btn;
	UPROPERTY(meta=(BindWidget))
	UAmountDeltaButton* MaxBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* SellConfirmBtn;
#pragma endregion

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UShopSellItemUI> WB_SellItemBtn;

	
private:
	UPROPERTY()
	UInventory* Client_Inventory;

	//TMap도 Struct 객체라서 UPROPERTY로 엔진에
	//GC 추적 등록을 안해도 됨.
	const TMap<FName, FShopSellEntry>* SellableItemMap;
	
	//UI보관소
	UPROPERTY()
	TArray<UShopSellItemUI*> SellItemUIs;

	UPROPERTY()
	UShopSellItemUI* CurSelectedSellItemUI;

	bool bIsSyncComplete;

private:
	/*==Server에 SellContext로 보내기 위한 데이터들==*/
	UPROPERTY()
	const UItemInstance* LastSelectWantSellItem;
	int LastSelectWantSellItemInventoryIdx;
	int LastSelectSlotUIIndex;
	int WantSellAmount;

	/*==서버보내기용은 아님==
	 * 최종가격이나 개당가격은 서버의 판매목록 데이터를 조회하여
	   서버에서 직접 따로 계산하기.
	*/
	int LastSelectWantSellItemPerPrice;
	
private:
	void UpdateSellItemList();
	void MakeSellItemUI();
	void OpenSellQuantityTab(
		const UItemInstance* ItemInstance,
		int InventoryIndex, int SlotIndex,int PerPrice);


	void ChangeCurSelectAmount(int Delta);
	UFUNCTION()
	void SellConfirm();
};
