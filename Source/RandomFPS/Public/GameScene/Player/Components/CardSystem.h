// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardSystem.generated.h"


class UCardAbility;
class UCardData;


DECLARE_MULTICAST_DELEGATE(FOnTimeToSelectCard);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipCard, UCardData*);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API UCardSystem : public UActorComponent
{
	GENERATED_BODY()


public:
	FOnTimeToSelectCard OnTimeToSelectCard;
	FOnEquipCard OnEquipCard;

public:	
	UCardSystem();
	void SelectCard();
	TArray<UCardData*>* Client__GetShuffleCard();
	UFUNCTION(Server,Reliable)
	void Server_SelectCard(FName CardID);
	UFUNCTION(Client,Reliable)
	void Client_FetchCards(FName CardID);
	
protected:
	virtual void BeginPlay() override;



private:
	//데이터용
	UPROPERTY(EditAnywhere)
	TArray<UCardData*> CardDatas;


	UPROPERTY()
	TArray<UCardData*> Server_Cards;
	UPROPERTY()
	TArray<UCardData*> Client_Cards;
	
	//server전용
	UPROPERTY()
	TArray<UCardAbility*> CardAbilities;
	
private:
	UFUNCTION(Client, Reliable)
	void Client_OpenCardMenu();
		
};
