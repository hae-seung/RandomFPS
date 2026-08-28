// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/CardSystem.h"

#include "GameScene/Cards/CardData.h"
#include "GameScene/Player/PlayerCharacter.h"


UCardSystem::UCardSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UCardSystem::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		Server_Cards = CardDatas;
	}

	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(APC->IsLocallyControlled())
		{
			Client_Cards = CardDatas;
		}
	}
}

//server
void UCardSystem::SelectCard()
{
	if(APlayerCharacter* APC = Cast<APlayerCharacter>(GetOwner()))
	{
		if(APC->IsLocallyControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("Client Request BroadCast"));
			OnTimeToSelectCard.Broadcast();
		}
		else
		{
			Client_OpenCardMenu();
		}
	}
}

void UCardSystem::Client_OpenCardMenu_Implementation()
{
	if(GetOwner()->HasAuthority())
		return;
	
	OnTimeToSelectCard.Broadcast();
}

TArray<UCardData*>* UCardSystem::Client__GetShuffleCard()
{
	for(int i = Client_Cards.Num() - 1; i > 0; i--)
	{
		const int j = FMath::RandRange(0, i);
		Client_Cards.Swap(i, j);
	}

	return &Client_Cards;
}

void UCardSystem::Server_SelectCard_Implementation(FName CardID)
{
	int Index = Server_Cards.IndexOfByPredicate(
		[CardID](UCardData* CardData)
	{
		return CardData && CardData->GetCardId() == CardID;
	});

	if(Index != INDEX_NONE)
	{
		UE_LOG(LogTemp,Warning, TEXT("Find Server Card"));
		//카드 획득으로 능력 적용
		CardAbilities.Add(Server_Cards[Index]->GetCardInstance(
			Cast<APlayerCharacter>(GetOwner())));
		
		//데이터 제거
		//-> 데이터를 안지우면 클라에서 (해커가)중복으로 들어온 카드를 중복으로 능력 획득함
		Server_Cards.RemoveAt(Index);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("No Server Card Index"));
	}

	
	Client_FetchCards(CardID);
}

void UCardSystem::Client_FetchCards_Implementation(FName CardID)
{
	UE_LOG(LogTemp,Warning,TEXT("Client Card Check"));
	UE_LOG(LogTemp, Warning, TEXT("CardName: %s"), *CardID.ToString());
	
	int Index = Client_Cards.IndexOfByPredicate(
		[CardID](UCardData* CardData)
		{
			return CardData && CardData->GetCardId() == CardID;
		});

	if(Index != INDEX_NONE)
	{
		UE_LOG(LogTemp,Warning,TEXT("Index IsValid"));
		OnEquipCard.Broadcast(Client_Cards[Index]);
		Client_Cards.RemoveAt(Index);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Index Is Not Valid"));
	}
}


