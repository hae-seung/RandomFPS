// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WrapBox.h"
#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/Inventory.h"
#include "GameScene/Player/ItemData/PartsData/RailPartsData.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "Public/UI/InventorySlot.h"
#include "UI/GunSlotUI.h"
#include "UI/TooltipUI.h"

void UInventoryUI::Toggle()
{
	bIsOpen = !bIsOpen;
	if(bIsOpen)
	{
		SetVisibility(ESlateVisibility::Visible);
		AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayer());
		PC->SetInputModeUI();
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayer());
		PC->SetInputModeGame();
	}
}


void UInventoryUI::Init()
{
	SetVisibility(ESlateVisibility::Collapsed);

	GunSlotUI->Init(this);
	
	APlayerCharacter* Character = GetOwningPlayer()->GetPawn<APlayerCharacter>();
	if(Character)
	{
		Inventory = Character->GetInventory();
		if(!Inventory) return;

		//로컬
		Inventory->RequestInitInventory(this);
	}
}




bool UInventoryUI::CheckInit() const
{
	return InventorySlots.Num() >= 1;
}


void UInventoryUI::UpdateUI(int Index)
{
	UItemInstance* Item = Inventory->InventoryList.Items[Index].ItemInstance;
	if(!InventorySlots.IsValidIndex(Index))
	{
		UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(this, BP_InventorySlot);
		NewSlot->Init(Item, Index, DragSlot);

		BindSlotEvents(NewSlot);
		
		InventoryList->AddChild(NewSlot);
		InventorySlots.Add(NewSlot);
	}
	else
	{
		InventorySlots[Index]->UpdateSlot(Item);
	}
}


void UInventoryUI::OpenToolTip(FVector2D ScreenPos, UItemInstance* Item)
{
	TooltipUI->OpenToolTip(Item);
	SetToolTipPos(ScreenPos);
}

void UInventoryUI::CloseToolTip()
{
	TooltipUI->CloseToolTip();
}

void UInventoryUI::SetToolTipPos(FVector2D ToolTipScreenPos)
{
	//UI는 좌상단이 (x(→):0, y(↓):0) 2차원 평면으로 생각
	
	if (!TooltipUI || !InventoryList || !InventoryBorder) return;

	// 툴팁 내용(SetText/SetBrush)까지 끝난 다음 호출된다는 전제.
	// 지금 프레임에서 정확한 DesiredSize가 필요하므로 Prepass.
	ForceLayoutPrepass();
	TooltipUI->ForceLayoutPrepass();

	const FVector2D TipSize = TooltipUI->GetDesiredSize();

	// Geometry 준비
	const FGeometry RootGeo = GetCachedGeometry();					  // 해당 BP의 루트는 결국 CanvasPanel임.
	const FGeometry BorderGeo = InventoryBorder->GetCachedGeometry(); // Border 기준

	// 1) Screen(Absolute) -> Border Local
	FVector2D BorderLocalPos = BorderGeo.AbsoluteToLocal(ToolTipScreenPos);

	// 기본 정책: 슬롯 "아래"로 조금 띄움
	constexpr float sPadding = 6.f;
	BorderLocalPos.Y += sPadding;

	const FVector2D BorderSize = BorderGeo.GetLocalSize();

	// 2) 아래로 배치했는데 바닥을 넘으면 "위"로 뒤집기 (선택이지만 UX 좋음)
	if (BorderLocalPos.Y + TipSize.Y > BorderSize.Y)
	{
		// 슬롯 기준점이 '아래 모서리'라고 가정하면 위로 올릴 때는 TipSize + Padding 만큼 위로
		BorderLocalPos.Y -= (TipSize.Y + sPadding);
	}

	// 3) Border 영역 안에서 Clamp
	BorderLocalPos.X = FMath::Clamp(BorderLocalPos.X, 0.f, FMath::Max(0.f, BorderSize.X - TipSize.X));
	BorderLocalPos.Y = FMath::Clamp(BorderLocalPos.Y, 0.f, FMath::Max(0.f, BorderSize.Y - TipSize.Y));

	// 4) Border Local -> InventoryUI(Local)로 변환해서 CanvasPanelSlot에 넣기
	// Border의 "절대 위치"를 InventoryUI 로컬로 변환해서 기준점으로 사용
	const FVector2D BorderOriginInRootLocal = RootGeo.AbsoluteToLocal(BorderGeo.GetAbsolutePosition());
	const FVector2D FinalPosInRootLocal = BorderOriginInRootLocal + BorderLocalPos;
	
	if (UCanvasPanelSlot* TipCanvasSlot = Cast<UCanvasPanelSlot>(TooltipUI->Slot))
	{
		TipCanvasSlot->SetAutoSize(true);     // 툴팁이 내용 기반 크기면 필수
		TipCanvasSlot->SetPosition(FinalPosInRootLocal); //CanvasPanel 기준의 좌표 위치로 최종 이동
		TipCanvasSlot->SetZOrder(999);
	}
}

void UInventoryUI::SwapItems(int From, int To)
{
	Inventory->RequestSwapItems(From, To);
}

void UInventoryUI::HandleSlotItem(int32 SlotIndex)
{
	Inventory->RequestHandleItem(SlotIndex);
}


void UInventoryUI::BindSlotEvents(UInventorySlot* NewSlot)
{
	NewSlot->OpenToolTipEvent.BindUObject(this, &UInventoryUI::OpenToolTip);
	NewSlot->HandleSlotItemEvent.BindUObject(this, &UInventoryUI::HandleSlotItem);
	NewSlot->CloseToolTipEvent.BindUObject(this, &UInventoryUI::CloseToolTip);
	NewSlot->SwapItemEvent.BindUObject(this, &UInventoryUI::SwapItems);
}


void UInventoryUI::UpdateGunSlot(UGunItem* CurrentEquipGun)
{
	GunSlotUI->UpdateNewGun(CurrentEquipGun);
}

void UInventoryUI::UpdatePartsSlot(EPartsType PartsType, UTexture2D* Icon)
{
	GunSlotUI->UpdateNewParts(PartsType, Icon);
}


UItemInstance* UInventoryUI::GetItemFromSlotIndex(int SlotIndex) const
{
	return Inventory->GetItemFromSlotIndex(SlotIndex);
}


void UInventoryUI::EquipDraggedItem(int Index)
{
	//인벤슬롯 하이라이트 끄고 장착
	InventorySlots[Index]->OffHighlight();
	HandleSlotItem(Index);
}

void UInventoryUI::RequestUnEquipParts(EPartsType PartsType)
{
	Inventory->RequestUnEquipParts(PartsType);
}

void UInventoryUI::SetGunRenderTarget(UTextureRenderTarget2D* RT)
{
	GunSlotUI->SetGunRenderTarget(RT);
}
