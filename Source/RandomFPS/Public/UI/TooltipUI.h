// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipUI.generated.h"

class UItemInstance;
class UOverlay;
class UTextBlock;
class UImage;


UCLASS()
class RANDOMFPS_API UTooltipUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void OpenToolTip(UItemInstance* Item);
	void CloseToolTip();

private:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameTxt;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDescriptionTxt;
	UPROPERTY(meta=(BindWidget))
	UOverlay* UseConfirmSet;

private:
	virtual void NativeOnInitialized() override;
};
