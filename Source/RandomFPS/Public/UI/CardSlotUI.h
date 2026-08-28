// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardSlotUI.generated.h"


class UCardData;
class UImage;

DECLARE_DELEGATE_TwoParams(FOnMouseEnterSlot, FText, FVector2D);
DECLARE_DELEGATE(FOnMouseLeaveSlot);


UCLASS()
class RANDOMFPS_API UCardSlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnMouseEnterSlot OnMouseEnterSlot;
	FOnMouseLeaveSlot OnMouseLeaveSlot;

	
public:
	void SetCard(UCardData* NewCardData);


protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	
private:
	UPROPERTY(meta=(BindWidget))
	UImage* CardIcon;
	

private:
	UPROPERTY()
	UCardData* CardData;
	
};
