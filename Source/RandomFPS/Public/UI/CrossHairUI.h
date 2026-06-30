// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrossHairUI.generated.h"

class UPlayerCombatSystem;
class UImage;



UCLASS()
class RANDOMFPS_API UCrossHairUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UPlayerCombatSystem* CombatSystem);
	
	void MoveCrossHair();
	
private:
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	UImage* TopCrosshair;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	UImage* RightCrosshair;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	UImage* DownCrosshair;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	UImage* LeftCrosshair;
	
	UPROPERTY(meta=(BindWidget))
	UImage* HitHair;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* HitEffectAnim;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* CriticalHitEffectAnim;

private:
	void HitSuccess(bool bIsCritical);
};
