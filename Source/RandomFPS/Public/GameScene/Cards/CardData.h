// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "CardData.generated.h"

/**
 * 
 */

class APlayerCharacter;
class UCardAbility;

UCLASS()
class RANDOMFPS_API UCardData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Card")
	FText CardName;
	UPROPERTY(EditAnywhere ,Category="Card", meta=(MultiLine=true))
	FText Description;
	UPROPERTY(EditAnywhere, Category="Card")
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere, Category="Card")
	ECardClass CardClass;
	UPROPERTY(EditAnywhere, Instanced, Category="Card")
	TObjectPtr<UCardAbility> CardInstance;




public:
	FText GetCardName();
	FText GetCardDescription();
	UTexture2D* GetCardIcon();
	FName GetCardId();
	TObjectPtr<UCardAbility> GetCardInstance(APlayerCharacter* APC);
};
