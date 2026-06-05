// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GunSlotUI.h"

#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameScene/Player/ItemData/PartsItemData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "GameScene/Player/ItemInstance/PartsItem.h"
#include "UI/DragDropInventory.h"
#include "UI/InventoryUI.h"
#include "UI/PartsSlotUI.h"


void UGunSlotUI::Init(UInventoryUI* InvenUI)
{
	InventoryUI = InvenUI;
	PartsSlotMap.Add(EPartsType::Rail, RailPartsSlot);
	PartsSlotMap.Add(EPartsType::Muzzle, MuzzlePartsSlot);
	PartsSlotMap.Add(EPartsType::Magazine, MagazinePartsSlot);

	for(auto& Pair : PartsSlotMap)
	{
		Pair.Value->BindRightMouseButtonEvent(FRightMouseButtonClickedEvent::CreateUObject(
			this,
			&UGunSlotUI::OnPartsSlotMouseRightButtonClicked));

		Pair.Value->Init();
	}
}


void UGunSlotUI::UpdateNewGun(UGunItem* NewGun)
{
	//현재 총에 따른 장착 가능 파츠 슬롯만 열고 나머지는 닫기
	for(auto& Pair : PartsSlotMap)
	{
		bool usable = NewGun->CheckPartsType(Pair.Key);
		Pair.Value->SetSlotUsable(usable);
	}
}

void UGunSlotUI::UpdateNewParts(EPartsType PartsType, UTexture2D* Icon)
{
	PartsSlotMap[PartsType]->UpdateImage(Icon);
}

void UGunSlotUI::SetGunRenderTarget(UTextureRenderTarget2D* RT)
{
	UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMat, this);
	MID->SetTextureParameterValue(TEXT("RenderTarget"), RT);
	GunPreviewImage->SetBrushFromMaterial(MID);
}


void UGunSlotUI::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                   UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	
	//하이라이트 조건
	//총인지 또는 파츠인지에 따라

	UDragDropInventory* DragDropInventory = Cast<UDragDropInventory>(InOperation);
	if(DragDropInventory)
	{
		UItemInstance* Item = InventoryUI->GetItemFromSlotIndex(DragDropInventory->SlotIndex);

		//총인지 파츠인지 분리
		//파츠면 map을 이용하여 일치하는 파츠 슬롯 찾기
		//해당 파츠 슬롯이 총에 대하여 장착가능한 파츠인지 확인. => 총이 교체되면 슬롯들도 활성, 비활성화 됨
		UGunItem* GunItem = Cast<UGunItem>(Item);
		if(GunItem)
		{
			HighlightImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			UPartsItem* PartsItem = Cast<UPartsItem>(Item);
			if(PartsItem && PartsSlotMap[PartsItem->GetPartsType()]->CheckCanSlotUsable())
			{
				PartsSlotMap[PartsItem->GetPartsType()]->SetHighlight();
				CurrentHighlightPartsSlotUI = PartsSlotMap[PartsItem->GetPartsType()];
			}
		}
	}
	
	
}

void UGunSlotUI::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	
	//총 하이라이트는 그냥 끄기
	//current highlight 끄기

	HighlightImage->SetVisibility(ESlateVisibility::Collapsed);
	if(CurrentHighlightPartsSlotUI != nullptr)
	{
		CurrentHighlightPartsSlotUI->OffHighlight();
		CurrentHighlightPartsSlotUI = nullptr;
	}
}

bool UGunSlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragDropInventory* DragOp = Cast<UDragDropInventory>(InOperation);
	if(!DragOp) return false;

	UItemInstance* Item = InventoryUI->GetItemFromSlotIndex(DragOp->SlotIndex);
	if(Item)
	{
		InventoryUI->EquipDraggedItem(DragOp->SlotIndex);
		HighlightImage->SetVisibility(ESlateVisibility::Collapsed);
		if(CurrentHighlightPartsSlotUI != nullptr)
		{
			CurrentHighlightPartsSlotUI->OffHighlight();
			CurrentHighlightPartsSlotUI = nullptr;
		}
	}

	//cancel로 남겨야 dragOp도 사라짐
	return false;
}

void UGunSlotUI::OnPartsSlotMouseRightButtonClicked(EPartsType PartsType)
{
	InventoryUI->RequestUnEquipParts(PartsType);
}





