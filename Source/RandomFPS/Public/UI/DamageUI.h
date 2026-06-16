// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageUI.generated.h"

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
	void Init(UWidgetComponent* WidgetComponent);
	void SetDamage(float DamageAmount, bool bIsCritical);

private:
	UPROPERTY()
	APlayerController* PC;
	UPROPERTY()
	UWidgetComponent* DamageWidgetComponent;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DamageText;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* PrintDamageAnim;
	
	FVector CameraLocation;
	FRotator CameraRotator;
	FRotator LookAtRot;

private:
	void LookCamera();
};
