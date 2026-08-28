// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Cards/CardData.h"

#include "GameScene/Cards/CardAbility/Base/CardAbility.h"

FText UCardData::GetCardName()
{
	return CardName;
}

FText UCardData::GetCardDescription()
{
	return Description;
}

UTexture2D* UCardData::GetCardIcon()
{
	return Icon;
}

FName UCardData::GetCardId()
{
	return GetFName();
}

TObjectPtr<UCardAbility> UCardData::GetCardInstance(APlayerCharacter* APC)
{
	UCardAbility* NewCard = CardInstance->Clone();
	NewCard->SetData(this);
	NewCard->OnAcquire(APC);
	
	return NewCard;
}
