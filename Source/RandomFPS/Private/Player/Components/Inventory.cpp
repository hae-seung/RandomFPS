// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/Inventory.h"

#include "Engine/ActorChannel.h"
#include "GameScene/Player/Components/PlayerWeapon.h"
#include "GameScene/Player/ItemData/PartsItemData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "GameScene/Player/ItemInstance/PartsItem/RailPartsItem.h"
#include "Net/UnrealNetwork.h"
#include "UI/InventoryUI.h"


UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();
}	

void UInventory::SetComponents(UPlayerWeapon* Weapon)
{
	PlayerWeapon = Weapon;
}


bool UInventory::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWrote = false;

	for(const FInventoryItem& Item : InventoryList.Items)
	{
		if(Item.ItemInstance)
		{
			bWrote |= Channel->ReplicateSubobject(
				Item.ItemInstance, *Bunch, *RepFlags);
		}
	}

	return bWrote;
}

UItemInstance* UInventory::GetItemFromSlotIndex(int SlotIndex)
{
	return InventoryList.Items[SlotIndex].ItemInstance;
}


void UInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventory, InventoryList);
}

void UInventory::RequestInitInventory(UInventoryUI* MyInventoryUI)
{
	InventoryUI = MyInventoryUI;
	InventoryList.Owner = this;
	
	if(!GetOwner()->HasAuthority())
	{
		Server_InitInventory();
	}
	else
	{
		InitInventory();
		for(int i = 0; i<SlotSize; i++)
		{
			InventoryUI->UpdateUI(i);
		}
	}
}


void UInventory::Server_InitInventory_Implementation()
{
	InitInventory();
	
}

void UInventory::InitInventory()
{
	for(int i = 0; i<SlotSize; i++)
	{
		FInventoryItem& NewItem = InventoryList.Items.AddDefaulted_GetRef();
		NewItem.SlotIndex = i;
		NewItem.ItemInstance = nullptr;
		InventoryList.MarkItemDirty(NewItem);
	}
}


void UInventory::OnRep_AddItem(int Index)
{
	InventoryUI->UpdateUI(Index);
}


//OnlyServer
int UInventory::AddItem(UItemInstance* Item, int OriginAmount)
{
	int index = -1;
	int Amount = OriginAmount;
	
	if(Item->GetMaxAmount() > 1)
	{
		bool bFindNextCountable = true;
		
		while(Amount > 0)
		{
			if(bFindNextCountable)
			{
				index = FindCountableItemSlotIndex(Item, index + 1);

				if(index == -1)
				{
					bFindNextCountable = false;
				}
				else
				{
					Amount = InventoryList.Items[index].ItemInstance->AddAmountAndGetExcess(Amount);
					
					//서버이자 로컬인 호스트도 update (FastArray의 replicate가 서버이자 로컬인 호스트는 안되기 때문)
					UpdateSlot(index);
				}
			}
			else
			{
				index = FindEmptySlot();
				if(index == -1)
				{
					return Amount;
				}
				
				//clone and add
				UItemInstance* CloneItem = Item->CloneItem();
				if(CloneItem)
				{
					CloneItem->SetAmount(Amount);
					InventoryList.Items[index].ItemInstance = CloneItem;
					InventoryList.MarkItemDirty(InventoryList.Items[index]);
						
					Amount = (Amount > CloneItem->GetMaxAmount()) ? Amount - CloneItem->GetMaxAmount() : 0;

					//호스트 전용
					UpdateSlot(index);
				}
			}
		}
	}
	else
	{
		index = FindEmptySlot();
		if(index != -1)
		{
			//clone and add
			UItemInstance* CloneItem = Item->CloneItem();
			if(CloneItem)
			{
				CloneItem->SetAmount(Amount);
				InventoryList.Items[index].ItemInstance = CloneItem;
				InventoryList.MarkItemDirty(InventoryList.Items[index]);
				Amount = 0;
				UpdateSlot(index);
			}
		}
	}

	if(OriginAmount > Amount)
	{
		OnItemAdd.Broadcast(Item->GetItemId(), OriginAmount - Amount);
	}
	
	return Amount;
}

int32 UInventory::FindCountableItemSlotIndex(UItemInstance* Item ,int Index)
{
	const TArray<FInventoryItem>& List = InventoryList.Items;
	for(int i = Index; i<List.Num(); i++)
	{
		if(!IsValid(List[i].ItemInstance))
			continue;
				
		if(Item->GetItemId() == List[i].ItemInstance->GetItemId() &&
			!List[i].ItemInstance->IsMax())
		{
			return i;
		}
	}

	return -1;
}

int32 UInventory::FindEmptySlot()
{
	const TArray<FInventoryItem>& List = InventoryList.Items;
	for(int i = 0; i<List.Num(); i++)
	{
		if(List[i].ItemInstance == nullptr)
			return i;
	}
	return -1;
}

void UInventory::UpdateSlot(int Index)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if(Pawn && Pawn->IsLocallyControlled())
	{
		//ui는 로컬에만 존재
		InventoryUI->UpdateUI(Index);
	}
}


void UInventory::OnRep_ItemInstanceStateChanged(UItemInstance* Item)
{
	//?
}


void UInventory::RequestSwapItems(int From, int To)
{
	//From과 To는 무조건 다름.
	APawn* Pawn = Cast<APawn>(GetOwner());
	if(Pawn && Pawn->HasAuthority())
	{
		SwapItems(From, To);
		
		UpdateSlot(From);
		UpdateSlot(To);
	}
	else
	{
		Server_SwapItems(From, To);
	}
}


void UInventory::Server_SwapItems_Implementation(int From, int To)
{
	SwapItems(From, To);
}

void UInventory::SwapItems(int From, int To)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, TEXT("Real Swaping"));
	InventoryList.UnBindItemEvent(From);
	InventoryList.UnBindItemEvent(To);

	Swap(InventoryList.Items[From].ItemInstance, InventoryList.Items[To].ItemInstance);

	InventoryList.MarkItemDirty(InventoryList.Items[From]);
	InventoryList.MarkItemDirty(InventoryList.Items[To]);
}

void UInventory::RequestHandleItem(int SlotIndex)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if(Pawn && Pawn->HasAuthority())
	{
		HandleItem(SlotIndex);
	}
	else
	{
		Server_HandleItem(SlotIndex);
	}
}

void UInventory::HandleItem(int SlotIndex)
{
	UItemInstance* InventoryItem = InventoryList.Items[SlotIndex].ItemInstance;
	if(InventoryItem == nullptr)
		return;

	if(UGunItem* GunItem = Cast<UGunItem>(InventoryItem))
	{
		EquipGun(GunItem, SlotIndex);
		return;
	}

	//todo: 파츠, 소모아이템 
	if(UPartsItem* PartsItem = Cast<UPartsItem>(InventoryItem))
	{
		EquipParts(PartsItem, SlotIndex);
		return;
	}

	
}

void UInventory::Server_HandleItem_Implementation(int SlotIndex)
{
	HandleItem(SlotIndex);
}

//server에서 일어남
void UInventory::EquipGun(UGunItem* GunItem, int SlotIndex)
{
	UGunItem* Temp = PlayerWeapon->GetCurrentEquipGun();

	if(Temp != nullptr)
		PlayerWeapon->UnEquipGun();
	
	//총 장착
	bool isSuccess = PlayerWeapon->EquipGun(GunItem);
	if(!isSuccess)
		return;

	//Client의 총 슬롯 업데이트
	Client_UpdateGunSlot(PlayerWeapon->GetCurrentEquipGun());
	
	//장착하던 기존의 총은 인벤토리로. 없었다면 알아서 null.
	InventoryList.Items[SlotIndex].ItemInstance = Temp;
	InventoryList.MarkItemDirty(InventoryList.Items[SlotIndex]);
	
	UpdateSlot(SlotIndex);
}

void UInventory::Client_UpdateGunSlot_Implementation(UGunItem* CurrentGun)
{
	InventoryUI->UpdateGunSlot(CurrentGun);
}


//Server
void UInventory::EquipParts(UPartsItem* PartsItem, int SlotIndex)
{
	UGunItem* TempGun = PlayerWeapon->GetCurrentEquipGun();
	
	//장착중인 총이 없다면 파츠 장착 불가
	if(TempGun == nullptr)
		return;
	
	
	//기존에 장착중인 파츠 확인
	EPartsType PartsType = PartsItem->GetPartsType();

	if(!TempGun->CheckPartsType(PartsType))
	{
		return;
	}
	
	UPartsItem* TempParts = PlayerWeapon->GetCurrentEquipParts(PartsType);
	if(TempParts != nullptr)
	{
		PlayerWeapon->UnEquipParts(PartsType);
	}
	
	//해당 파츠가 해당 총에 장착 불가능 파츠일수 있음
	if(PlayerWeapon->EquipParts(PartsItem))
	{
		InventoryList.Items[SlotIndex].ItemInstance = TempParts;
		InventoryList.MarkItemDirty(InventoryList.Items[SlotIndex]);
		Client_UpdatePartsSlot(PartsType, PartsItem->GetPartsItemData()->GetItemIcon());
	}
	
	UpdateSlot(SlotIndex);
}

void UInventory::Client_UpdatePartsSlot_Implementation(EPartsType PartsType, UTexture2D* Icon)
{
	InventoryUI->UpdatePartsSlot(PartsType, Icon);
}

void UInventory::RequestUnEquipParts(EPartsType PartsType)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if(Pawn && Pawn->HasAuthority())
	{
		UnEquipParts(PartsType);
	}
	else
	{
		Server_UnEquipParts(PartsType);
	}
}

void UInventory::Server_UnEquipParts_Implementation(EPartsType PartsType)
{
	UnEquipParts(PartsType);
}

//server
void UInventory::UnEquipParts(EPartsType PartsType)
{
	UPartsItem* Temp = PlayerWeapon->GetCurrentEquipParts(PartsType);

	const int remain = AddItem(Temp, 1);
	if(remain >= 1)
		return;

	PlayerWeapon->UnEquipParts(PartsType);

	//client 파츠 슬롯 업데이트
	Client_UpdatePartsSlot(PartsType);
}

//Server
int UInventory::GetItemTotalAmount(UItemData* ItemId)
{
	int ItemAmount = 0;
	const TArray<FInventoryItem>& List = InventoryList.Items;
	const FName& ItemRealId = ItemId->GetItemId();
	for(int i = 0; i < List.Num(); i++)
	{
		if(IsValid(List[i].ItemInstance) && List[i].ItemInstance->GetItemId().IsEqual(ItemRealId))
		{
			ItemAmount += List[i].ItemInstance->ItemAmount;
		}
	}

	return ItemAmount;
}

void UInventory::RemoveItemFromId(FName ItemId, int Amount)
{
	TArray<FInventoryItem>& List = InventoryList.Items;
	for(int i = 0; i < List.Num(); i++)
	{
		if(Amount <= 0)
			break;
		
		if(!IsValid(List[i].ItemInstance))
			continue;

		UItemInstance* CItem = List[i].ItemInstance;
		if(CItem->GetItemId().IsEqual(ItemId))
		{
			if(CItem->ItemAmount <= Amount)
			{
				List[i].ItemInstance = nullptr;
				InventoryList.MarkItemDirty(InventoryList.Items[i]);
				Amount -= CItem->ItemAmount;
			}
			else
			{
				CItem->SetAmount(CItem->ItemAmount - Amount);
				Amount = 0;
			}
		}

		UpdateSlot(i);
	}
}
