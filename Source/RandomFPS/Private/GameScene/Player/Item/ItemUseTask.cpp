// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Item/ItemUseTask.h"

#include "GameFramework/GameStateBase.h"
#include "GameScene/Player/Components/PlayerInteractSystem.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "Interface/Useable.h"

void UItemUseTask::Init(
	UPlayerInteractSystem* UserInteractSystem,
	UPlayerStatSystem* PlayerStatSystem)
{
	InteractSystem = UserInteractSystem;
	StatSystem = PlayerStatSystem;

	InteractSystem->OnInteractEnd.AddUObject(this, &UItemUseTask::EndUseItem);
}


void UItemUseTask::RegisterItem(UItemInstance* UsableItem, int idx)
{
	if(InteractSystem->IsInteracting())
	{
		return;
	}

	Index = idx;
	StartUseItem(UsableItem);
}

void UItemUseTask::StartUseItem(UItemInstance* Item)
{
	//player 행동 시키기 => 몽타주 실행
	IUseable* UsableItem = Cast<IUseable>(Item);
	
	//등록
	if(const float WaitTime = UsableItem->GetNeedTime())
	{
		BookedItem = Item;
		
		//캐릭터에게 알리기 => UI띄워줄거임
		float EndTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds() + WaitTime;
		UAnimMontage* PlayMontage = UsableItem->GetUsingMontage();
		UTexture2D* Icon = Item->ItemData->GetItemIcon();
		InteractSystem->StartInteract(PlayMontage, EndTime, WaitTime, Icon);
	}
	else
	{
		UsableItem->Use(StatSystem);
	}
}

void UItemUseTask::EndUseItem(bool bSuccess)
{
	/*아이템 사용으로 인한 상호작용중이 아닌 다른 행위의 종료로 인한
	상호작용 종료신호를 받아도 해당 함수가 실행됨.
	따라서 BookedItem은 아이템 사용중이 아닐땐 nullptr로 두기*/
	if(!IsValid(BookedItem))
		return;
	
	if(bSuccess)
	{
		RealUseItem();
	}
}

void UItemUseTask::RealUseItem()
{
	IUseable* Usable = Cast<IUseable>(BookedItem);
	Usable->Use(StatSystem);

	//서버의 아이템은 MarkItemDirty때문에 Inventory에서 사용결과를 처리해줘야함.
	if(Index != -1)
	{
		OnUseItemComplete.ExecuteIfBound(Index);
	}

	//사용 끝
	BookedItem = nullptr;
}
