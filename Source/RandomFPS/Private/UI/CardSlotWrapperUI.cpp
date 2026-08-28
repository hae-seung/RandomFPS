// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CardSlotWrapperUI.h"

#include "GameScene/Player/Components/CardSystem.h"
#include "UI/CardSlotUI.h"
#include "UI/InventoryUI.h"


void UCardSlotWrapperUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CardSlots.Add(CardSlot1);
	CardSlots.Add(CardSlot2);
	CardSlots.Add(CardSlot3);
	CardSlots.Add(CardSlot4);

	
	FillCardIndex = 0;
}


void UCardSlotWrapperUI::Init(UInventoryUI* InInventoryUI, UCardSystem* InCardSystem)
{
	InventoryUI = InInventoryUI;
	CardSystem = InCardSystem;

	for(int i = 0; i < CardSlots.Num(); i++)
	{
		CardSlots[i]->OnMouseEnterSlot.BindUObject(InventoryUI, &UInventoryUI::OpenCardToolTip);
		CardSlots[i]->OnMouseLeaveSlot.BindUObject(InventoryUI, &UInventoryUI::CloseCardToolTip);
	}
	
	CardSystem->OnEquipCard.AddUObject(this, &UCardSlotWrapperUI::SetCardSlot);
}

void UCardSlotWrapperUI::SetCardSlot(UCardData* NewCardData)
{
	if(FillCardIndex >= CardSlots.Num())
		return;

	UE_LOG(LogTemp,Warning,TEXT("SetCard Do"));
	CardSlots[FillCardIndex]->SetCard(NewCardData);
	FillCardIndex++;
}
