// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Item/ItemUseTask.h"

#include "GameFramework/GameStateBase.h"
#include "GameScene/Player/Components/PlayerInteractSystem.h"
#include "GameScene/Player/ItemData/ItemData.h"
#include "GameScene/Player/ItemInstance/ItemInstance.h"
#include "Interface/Useable.h"

void UItemUseTask::Init(UPlayerInteractSystem* UserInteractSystem, UPlayerStatSystem* PlayerStatSystem)
{
	InteractSystem = UserInteractSystem;
	StatSystem = PlayerStatSystem;
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
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this, &UItemUseTask::RealUseItem,
			WaitTime,
			false);
		
		//캐릭터에게 알리기 => UI띄움
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

void UItemUseTask::StopUseItem()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	//player 행동 강제 중지 시키기
	InteractSystem->StopInteractMontage();
}

void UItemUseTask::RealUseItem()
{
	//player 몽타주 End섹션 넘기기
	InteractSystem->SetMontageJumpEnd();
	
	IUseable* Usable = Cast<IUseable>(BookedItem);
	Usable->Use(StatSystem);

	//서버의 아이템은 MarkItemDirty때문에 Inventory에서 사용결과를 처리해줘야함.
	if(Index != -1)
	{
		OnUseItemComplete.ExecuteIfBound(Index);
	}
}
