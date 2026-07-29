// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GunAbilityDescriptionUI.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UGunAbilityDescriptionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetText(FText Description);
	
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DescriptionText;
};
