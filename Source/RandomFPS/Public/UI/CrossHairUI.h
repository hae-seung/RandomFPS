// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrossHairUI.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UCrossHairUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void MoveCorssHair();
	
private:
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	UImage* TopCrosshair;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	UImage* RightCrosshair;

	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	UImage* DownCrosshair;

	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	UImage* LeftCrosshair;
};
