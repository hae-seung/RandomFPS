// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopSellTabUI.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "GameScene/EnumHeader/FInventoryList.h"
#include "GameScene/Player/Components/Inventory.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "UI/AmountDeltaButton.h"
#include "UI/ShopSellItemUI.h"

void UShopSellTabUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	MinBtn->InitFirstOpen();
	Minus1Btn->InitFirstOpen();
	Minus10Btn->InitFirstOpen();
	Plus1Btn->InitFirstOpen();
	Plus10Btn->InitFirstOpen();
	MaxBtn->InitFirstOpen();
	
	MinBtn->OnCustomButtonReleased.AddUObject(this, &UShopSellTabUI::ChangeCurSelectAmount);
	Minus1Btn->OnCustomButtonReleased.AddUObject(this, &UShopSellTabUI::ChangeCurSelectAmount);
	Minus10Btn->OnCustomButtonReleased.AddUObject(this, &UShopSellTabUI::ChangeCurSelectAmount);
	Plus1Btn->OnCustomButtonReleased.AddUObject(this, &UShopSellTabUI::ChangeCurSelectAmount);
	Plus10Btn->OnCustomButtonReleased.AddUObject(this, &UShopSellTabUI::ChangeCurSelectAmount);
	MaxBtn->OnCustomButtonReleased.AddUObject(this, &UShopSellTabUI::ChangeCurSelectAmount);

	
	SellConfirmBtn->OnReleased.AddDynamic(this, &UShopSellTabUI::SellConfirm);

	bIsSyncComplete = true;
}

void UShopSellTabUI::FirstOpenInit(UInventory* Inventory,
	const TMap<FName, FShopSellEntry>& SellEntryMap)
{
	Client_Inventory = Inventory;

	SellableItemMap = &SellEntryMap;
}

void UShopSellTabUI::SetSyncState(bool bState)
{
	bIsSyncComplete = bState;
}

void UShopSellTabUI::Open()
{
	SetVisibility(ESlateVisibility::Visible);

	//SellTab은 열릴때마다 이전에 팔려고 선택한 아이템 정보를 전부 초기화.
	//즉 열때마다 처음 탭을 연거처럼 계속 초기화 해야함
	SellQuantityTab->SetVisibility(ESlateVisibility::Collapsed);
	if(CurSelectedSellItemUI)
	{
		CurSelectedSellItemUI->SetCanClickState(true);
	}
	UpdateSellItemList();
}

void UShopSellTabUI::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}


void UShopSellTabUI::UpdateSellItemList()
{
	MakeSellItemUI();
	
	InventoryItemBox->ClearChildren();

	int SellItemTypeAmount = 0;
	const FInventoryList& InventoryList = Client_Inventory->InventoryList;
	for(const FInventoryItem& Item : InventoryList.Items)
	{
		if(IsValid(Item.ItemInstance))
		{
			SellItemTypeAmount++;
			UShopSellItemUI* SellItemUI = SellItemUIs[SellItemTypeAmount - 1];

			int PerPrice = 1;
			const FShopSellEntry* Entry = SellableItemMap->Find(Item.ItemInstance->GetItemId());
			if(Entry)
			{
				PerPrice = Entry->PerPrice;
			}
			SellItemUI->SetItemInfoAndOpen(Item, PerPrice);
			InventoryItemBox->AddChild(SellItemUI);
		}
	}

	for(int i = SellItemTypeAmount; i < SellItemUIs.Num(); i++)
	{
		SellItemUIs[i]->Close();
	}
}

void UShopSellTabUI::MakeSellItemUI()
{
	while(SellItemUIs.Num() < Client_Inventory->GetInventoryMaxSize())
	{
		UShopSellItemUI* NewSlot = CreateWidget<UShopSellItemUI>(this, WB_SellItemBtn);
		NewSlot->OnClickedSellItemUI.BindUObject(this, &UShopSellTabUI::OpenSellQuantityTab);
		SellItemUIs.Add(NewSlot);
		NewSlot->SetUIIndex(SellItemUIs.Num() - 1);
	}
}

void UShopSellTabUI::OpenSellQuantityTab(
	const UItemInstance* ItemInstance,
	int InventoryIndex, int SlotIndex, int PerPrice)
{
	if(!bIsSyncComplete)
	{
		//서버와 동기화가 아직 안됐으면 다른 판매 품목 클릭 막음.
		//이러면 LastSelectSlotUI는 서버로부터 응답이 올때까지
		//캐싱된 값이 변경되지 않음
		return;
	}
	
	
	if(CurSelectedSellItemUI)
	{
		CurSelectedSellItemUI->SetCanClickState(true);
	}
	CurSelectedSellItemUI = SellItemUIs[SlotIndex];
	CurSelectedSellItemUI->SetCanClickState(false);

	
	SellQuantityTab->SetVisibility(ESlateVisibility::Visible);

	LastSelectWantSellItem = ItemInstance;
	LastSelectWantSellItemInventoryIdx = InventoryIndex;
	LastSelectSlotUIIndex = SlotIndex;
	WantSellAmount = 0;
	LastSelectWantSellItemPerPrice = PerPrice;

	ItemNameText->SetText(ItemInstance->ItemData->GetItemName());
	ItemImage->SetBrushFromTexture(ItemInstance->ItemData->GetItemIcon());
	
	StockText->SetText(FText::Format(
		FText::FromString(TEXT("x{0}")),
		ItemInstance->ItemAmount));
	
	PerPriceText->SetText(FText::Format(
		FText::FromString(TEXT("{0}원")), PerPrice));

	SelectSellAmountText->SetText(FText::FromString(TEXT("0")));

	FinalPriceText->SetText(FText::FromString(TEXT("최종 판매가격 : 0원")));

	SellConfirmBtn->SetIsEnabled(false);
}

void UShopSellTabUI::ChangeCurSelectAmount(int Delta)
{
	const int CanMaxSellAmount = LastSelectWantSellItem->ItemAmount;
	
	WantSellAmount = FMath::Clamp(WantSellAmount + Delta, 0, CanMaxSellAmount);

	SelectSellAmountText->SetText(FText::AsNumber(WantSellAmount));
	
	FinalPriceText->SetText(FText::Format(
		FText::FromString(TEXT("최종 판매가격 : {0}원")),
		WantSellAmount * LastSelectWantSellItemPerPrice));

	SellConfirmBtn->SetIsEnabled(WantSellAmount > 0);
}


void UShopSellTabUI::SellConfirm()
{
	if(!bIsSyncComplete)
	{
		//서버와 동기화가 되지 않으면 팔기 불가능
		return;
	}

	const FShopSellContext Context{
		LastSelectWantSellItem->ItemData->GetItemId(),
		LastSelectWantSellItemInventoryIdx,
		WantSellAmount};

	bIsSyncComplete = false;
	
	OnSellConfirm.ExecuteIfBound(Context);
}

void UShopSellTabUI::UpdateSellTabByFeedback(const FShopSellContextFeedback& SellContextFeedback)
{
	UE_LOG(LogTemp, Warning,
		TEXT("ShopSellTabUI::SetSyncState = %s"),
		bIsSyncComplete ? TEXT("TRUE") : TEXT("FALSE"));
	
	if(SellContextFeedback.ItemRemainStock <= 0)
	{
		SellQuantityTab->SetVisibility(ESlateVisibility::Collapsed);
		CurSelectedSellItemUI->Close();
	}
	else
	{
		StockText->SetText(FText::Format(
		FText::FromString(TEXT("x{0}")),
		SellContextFeedback.ItemRemainStock));

		CurSelectedSellItemUI->SetRemainStock(SellContextFeedback.ItemRemainStock);
		WantSellAmount = 0;
		
		FinalPriceText->SetText(FText::FromString(TEXT("최종 판매가격 : 0원")));
		
		SelectSellAmountText->SetText(FText::AsNumber(WantSellAmount));

		SellConfirmBtn->SetIsEnabled(false);
	}
}
