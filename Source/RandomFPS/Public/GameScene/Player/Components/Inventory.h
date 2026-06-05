// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/EnumHeader/FInventoryList.h"

#include "Inventory.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemAdd, FName, int);

class UPartsItem;
class UPartsItemData;
class UPlayerWeapon;
class UGunItem;
class UItemData;
class UInventoryUI;
class UUIManager;
class AMyPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	FOnItemAdd OnItemAdd;
	
public:	
	UInventory();

	void SetComponents(UPlayerWeapon* Weapon);
	
	void RequestInitInventory(UInventoryUI* MyInventoryUI);
	void RequestSwapItems(int From, int To);
	
	void RequestHandleItem(int SlotIndex);
	
	
	
	void OnRep_AddItem(int Index);
	void UpdateSlot(int Index);

	//OnlyServer
	int AddItem(UItemInstance* Item, int Amount);
	int GetItemTotalAmount(UItemData* ItemId);
	void RemoveItemFromId(FName ItemId, int Amount = 1);
	
	//ActorComponent가 단독으로 호출은 불가하지만 Actor에서 여기로 호출은 가능함.
	//=> 엔진이 미리 구현해두었음. 이런경우를 생각해서
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	
	UItemInstance* GetItemFromSlotIndex(int SlotIndex);
	
	void RequestUnEquipParts(EPartsType PartsType);
	void UnEquipParts(EPartsType PartsType);
	UFUNCTION(Server,Reliable)
	void Server_UnEquipParts(EPartsType PartsType);
	
protected:
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditAnywhere)
	int32 SlotSize;

	//로컬만 보유중. 리모트는 null;
	UInventoryUI* InventoryUI;
	//서버 클라 모두 보유
	UPlayerWeapon* PlayerWeapon;
	
private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_InitInventory();
	void InitInventory();

	int32 FindCountableItemSlotIndex(UItemInstance* Item, int Amount);
	int32 FindEmptySlot();
	void OnRep_ItemInstanceStateChanged(UItemInstance* Item);

	void SwapItems(int From, int To);
	UFUNCTION(Server, Reliable)
	void Server_SwapItems(int From, int To);

	void HandleItem(int SlotIndex);
	UFUNCTION(Server,Reliable)
	void Server_HandleItem(int SlotIndex);

	void EquipGun(UGunItem* GunItem, int SlotIndex);
	UFUNCTION(Client, Reliable)
	void Client_UpdateGunSlot(UGunItem* CurrentGun);

	void EquipParts(UPartsItem* PartsItem, int SlotIndex);
	UFUNCTION(Client, Reliable)
	void Client_UpdatePartsSlot(EPartsType PartsType, UTexture2D* Icon = nullptr);
};
