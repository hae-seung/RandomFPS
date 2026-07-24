// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameScene/EnumHeader/FInventoryList.h"

#include "Inventory.generated.h"


class UPlayerInteractSystem;
class UItemUseTask;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemAdd, FName, int);

class UPartsItem;
class UPartsItemData;
class UPlayerWeapon;
class UGunItem;
class UItemData;
class UInventoryUI;
class UUIManager;
class AMyPlayerController;
class UPlayerStatSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnItemAdd OnItemAdd;
	
public:
	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	
public:	
	UInventory();

	void SetComponents(
		UPlayerWeapon* Weapon,
		UPlayerInteractSystem* InteractSystem,
		UPlayerStatSystem* StatSystem);
	
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
	virtual void InitializeComponent() override;

private:
	UPROPERTY(EditAnywhere)
	int32 SlotSize;
	
	UPROPERTY()
	UInventoryUI* InventoryUI;
	UPROPERTY()
	UPlayerWeapon* PlayerWeapon;
	UPROPERTY()
	UItemUseTask* ItemUseTask;

	
	
private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	void UseItemComplete(int SlotIndex);
	
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
