// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "UI/DragDropInventory.h"
#include "UI/DragSlot.h"


void UInventorySlot::NativeOnInitialized()
{
	HideSlot();
}

void UInventorySlot::Init(UItemInstance* Item, int Index, UDragSlot* DragSlot)
{
	DragWidget = DragSlot;
	SlotIndex = Index; //평생 고정값
	UpdateSlot(Item);
}


void UInventorySlot::UpdateSlot(UItemInstance* Item)
{
	SlotItem = Item;

	if (!IsValid(Item))
	{
		HideSlot();
		return;
	}

	if (!IsValid(Item->ItemData))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("NoITemData"));
		HideSlot();
		return;
	}

	if (!ItemImage || !ItemAmountTxt)
	{
		return;
	}

	ShowSlot();

	ItemImage->SetBrushFromTexture(Item->ItemData->GetItemIcon());
	ItemAmountTxt->SetText(FText::AsNumber(Item->ItemAmount));
}

void UInventorySlot::OnHighlight()
{
	HighLightImage->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInventorySlot::OffHighlight()
{
	HighLightImage->SetVisibility(ESlateVisibility::Collapsed);
}


void UInventorySlot::HideSlot()
{
	ItemImage->SetVisibility(ESlateVisibility::Collapsed);
	ItemAmountTxt->SetVisibility(ESlateVisibility::Collapsed);
	OffHighlight();
}

void UInventorySlot::ShowSlot()
{
	ItemImage->SetVisibility(ESlateVisibility::Visible);
	ItemAmountTxt->SetVisibility(ESlateVisibility::Visible);
}


void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnHighlight();
	
	if(SlotItem == nullptr)
		return;

	const FVector2D BottomLeftLocal(0.f, InGeometry.GetLocalSize().Y);
	const FVector2D ScreenPos = InGeometry.LocalToAbsolute(BottomLeftLocal);
	
	OpenToolTipEvent.ExecuteIfBound(ScreenPos, SlotItem);
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	OffHighlight();

	CloseToolTipEvent.ExecuteIfBound();
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		//마우스 왼쪽 버튼에 대한 드래그 감지 이벤트 등록
		return UWidgetBlueprintLibrary::DetectDragIfPressed(
			InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		HandleRightClickSlot();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	if(SlotItem == nullptr) return;

	DragWidget->SetImage(SlotItem->ItemData->GetItemIcon());
	UDragDropInventory* DragOp = NewObject<UDragDropInventory>(this);
	DragOp->Init(SlotIndex);
	DragOp->DefaultDragVisual = DragWidget;
	OutOperation = DragOp;
}

void UInventorySlot::NativeOnDragEnter(const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	
	UDragDropInventory* DragOp = Cast<UDragDropInventory>(InOperation);
	if(DragOp->SlotIndex == SlotIndex)
		return;
	
	OnHighlight();
}

void UInventorySlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//드래그 중인 마우스가 슬롯에서 나갈때
	OffHighlight();
}



bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragDropInventory* DragOp = Cast<UDragDropInventory>(InOperation);
	if(!DragOp) return false;

	const int From = DragOp->SlotIndex;
	const int To = SlotIndex;

	if(From == To) return false;

	SwapItemEvent.ExecuteIfBound(From, To);

	//cancel 상태로 남게 될거임.
	return false;
}


void UInventorySlot::HandleRightClickSlot()
{
	if(SlotItem == nullptr)
		return;

	HandleSlotItemEvent.ExecuteIfBound(SlotIndex);
}

