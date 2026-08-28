// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "InventoryUI.generated.h"

class UTextBlock;
class UOverlay;
class UCardSlotWrapperUI;
class UCardSlotUI;
class UPlayerWeapon;
class UStatUI;
class URailPartsData;
class UButton;
class UPartsItem;
class UGunSlotUI;
class UGunItem;
class UDragSlot;
class UBorder;
class UCanvasPanel;
class UItemInstance;
class UTooltipUI;
class UWrapBox;
class UInventorySlot;
class UInventory;
class UPlayerStatSystem;
class UCardSystem;

UCLASS()
class RANDOMFPS_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(
		UInventory* PlayerInventory,
		UPlayerStatSystem* StatSystem,
		UPlayerWeapon* PlayerWeapon,
		UCardSystem* CardSystem);
	void UpdateUI(int Index);
	bool CheckInit() const;
	void Toggle();
	void EquipDraggedItem(int Index);
	
	void UpdateGunSlot(UGunItem* CurrentEquipGun);
	void UpdatePartsSlot(EPartsType PartsType, UTexture2D* Icon = nullptr);
	UItemInstance* GetItemFromSlotIndex(int SlotIndex) const;

	void RequestUnEquipParts(EPartsType PartsType);

	void SetGunRenderTarget(UTextureRenderTarget2D* RT);

	
	void OpenCardToolTip(FText CardDescription, FVector2D ScreenPos);
	void CloseCardToolTip();

private:
	UPROPERTY(meta=(BindWidget))
	UBorder* InventoryBorder;
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryList;
	UPROPERTY(meta=(BindWidget))
	UTooltipUI* TooltipUI;
	UPROPERTY(meta=(BindWidget))
	UDragSlot* DragSlot;

	UPROPERTY(meta=(BindWidget))
	UStatUI* StatUI;
	UPROPERTY(meta=(BindWidget))
	UGunSlotUI* GunSlotUI;

	UPROPERTY(meta=(BindWidget))
	UCardSlotWrapperUI* CardSlotWrapperUI;
	UPROPERTY(meta=(BindWidget))
	UOverlay* CardTooltipUI;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CardTooltipText;
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* CanvasPanel;

private:
	UPROPERTY()
	UInventory* Inventory;
	UPROPERTY()
	TArray<UInventorySlot*> InventorySlots;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> BP_InventorySlot;


	bool bIsOpen = false;
	
private:
	void SetToolTipPos(FVector2D ToolTipScreenPos);
	void BindSlotEvents(UInventorySlot* NewSlot);
	

	void OpenToolTip(FVector2D ScreenPos, UItemInstance* Item);
	void HandleSlotItem(int32 SlotIndex);
	void CloseToolTip();
	void SwapItems(int From, int To);
	
	void SetCardToolTipPos(FVector2D ScreenPos);
};
