// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CardMenuUI.h"

#include "GameScene/Player/MyPlayerController.h"
#include "GameScene/Player/Components/CardSystem.h"
#include "UI/CardUI.h"

void UCardMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CardSets.Add(CardSet1);
	CardSets.Add(CardSet2);
	CardSets.Add(CardSet3);
	
	for(int i = 0; i < 3; i++)
	{
		CardSets[i]->OnCardSelected.BindUObject(this, &UCardMenuUI::SelectComplete);
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void UCardMenuUI::Init(UCardSystem* InCardSystem)
{
	CardSystem = InCardSystem;
	
	CardSystem->OnTimeToSelectCard.AddUObject(this, &UCardMenuUI::OpenCardMenu);
}

void UCardMenuUI::OpenCardMenu()
{
	UE_LOG(LogTemp,Warning,TEXT("OpenCardMenu"));
	SetVisibility(ESlateVisibility::Visible);

	AMyPlayerController* AMC = Cast<AMyPlayerController>(GetOwningPlayer());
	AMC->SetInputModeUI();
	
	TArray<UCardData*>* Cards = CardSystem->Client__GetShuffleCard();

	for(int i = 0; i < 3; i++)
	{
		if(i >= Cards->Num())
		{
			CardSets[i]->Hide();
			continue;
		}
		
		CardSets[i]->Show();
		CardSets[i]->SetCardInfo((*Cards)[i]);
	}
}

void UCardMenuUI::SelectComplete(FName ID)
{
	AMyPlayerController* AMC = Cast<AMyPlayerController>(GetOwningPlayer());
	AMC->SetInputModeGame();

	SetVisibility(ESlateVisibility::Collapsed);

	CardSystem->Server_SelectCard(ID);
}
