// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardUI.generated.h"

class USizeBox;
class UCardData;
class UImage;
class UTextBlock;
class UButton;

DECLARE_DELEGATE_OneParam(FOnCardSelected, FName);


UCLASS()
class RANDOMFPS_API UCardUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnCardSelected OnCardSelected;

	
public:
	void Show();
	void Hide();
	void SetCardInfo(UCardData* CardData);


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	

private:
	UPROPERTY(meta=(BindWidget))
	UButton* CardBtn;
	UPROPERTY(meta=(BindWidget))
	USizeBox* SizeBox;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CardNameText;
	UPROPERTY(meta=(BindWidget))
	UImage* CardIcon;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CardDescriptionText;

	UPROPERTY(EditAnywhere)
	FVector2D NormalSize;
	UPROPERTY(EditAnywhere)
	FVector2D BigSize;

private:
	UPROPERTY()
	FName CardID;


private:
	UFUNCTION()
	void SelectCardBtnClicked();
};
