// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthUI.generated.h"

class UWidgetComponent;

UCLASS()
class RANDOMFPS_API UHealthUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UWidgetComponent* HealthBarWidget);
	void HideHealth();
	void UpdateHealth(float Hp, float MaxHp);
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	
private:
	UPROPERTY()
	APlayerController* PC;
	UPROPERTY()
	UWidgetComponent* HealthBarWidgetComp;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;
	
	bool bIsOpen;
	FVector CameraLocation;
	FRotator CameraRotation;
	FRotator LookAtRot;

private:
};
