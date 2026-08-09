// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "ShopCategoryBtn.generated.h"


class UButton;
class UTextBlock;
class UShopCategoryBtn;

DECLARE_DELEGATE_TwoParams(FOnCategoryBtnClicked, EShopCategoryType, UShopCategoryBtn*);


UCLASS()
class RANDOMFPS_API UShopCategoryBtn : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnCategoryBtnClicked OnCategoryBtnClicked;

public:
	void Init(EShopCategoryType CategoryType, FText ButtonName);
	void SetSelectState(bool bState);

	
protected:
	virtual void NativeOnInitialized() override;

	
private:
	UPROPERTY(meta=(BindWidget))
	UButton* CategoryBtn;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CategoryNameText;


private:
	EShopCategoryType ShopCategoryType;

private:
	UFUNCTION()
	void OpenCategoryList();
	
};
