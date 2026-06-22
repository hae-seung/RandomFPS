// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageUI.generated.h"

class ADamageActor;
class UWidgetComponent;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UDamageUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDamage(float DamageAmount, bool bIsCritical);

private:
	
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DamageText;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* PrintDamageAnim;


};
