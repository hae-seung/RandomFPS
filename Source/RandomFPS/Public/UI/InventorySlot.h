// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "InventorySlot.generated.h"

class UDragSlot;
class UTextBlock;
class UImage;
class UItemInstance;

DECLARE_DELEGATE(FCloseToolTipEvent);

DECLARE_DELEGATE_OneParam(FHandleSlotItemEvent, int32);

DECLARE_DELEGATE_TwoParams(FOpenToolTipEvent, FVector2D, UItemInstance*);
DECLARE_DELEGATE_TwoParams(FSwapItemEvent, int32, int32);

UCLASS()
class RANDOMFPS_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	FOpenToolTipEvent OpenToolTipEvent;
	FHandleSlotItemEvent HandleSlotItemEvent;
	FCloseToolTipEvent CloseToolTipEvent;
	FSwapItemEvent SwapItemEvent;

public:
	void Init(UItemInstance* Item, int Index, UDragSlot* DragSlot);
	void UpdateSlot(UItemInstance* Item);
	void OnHighlight();
	void OffHighlight();
	
	
private:
	UDragSlot* DragWidget;
	UItemInstance* SlotItem;
	int32 SlotIndex;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemAmountTxt;
	UPROPERTY(meta=(BindWidget))
	UImage* HighLightImage;

private:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	
	void HideSlot();
	void ShowSlot();

	void HandleRightClickSlot();
};
