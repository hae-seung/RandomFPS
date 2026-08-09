// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopUI.h"
#include "Blueprint/UserWidget.h"
#include "Struct/ShopStruct.h"
#include "ShopBuyTabUI.generated.h"

class UShopUI;
class UHorizontalBox;
class UShopItemUI;
class UWrapBox;
class UShopCategoryBtn;


UCLASS()
class RANDOMFPS_API UShopBuyTabUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Open();
	void Close();
	void FirstOpenInit(FShopState& ShopInstance, UShopUI* InShopUI);
	void UpdateShopItemByFeedback(const FShopBuyContextFeedback& BuyContextFeedback);

	
private:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess=true))
	UWrapBox* ItemList;
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* CategoryButtonBox;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UShopItemUI> WB_ShopItemUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UShopCategoryBtn> WB_CategoryBtn;
	UPROPERTY(EditAnywhere)
	FMargin CategoryButtonPadding;
	UPROPERTY(EditAnywhere)
	TMap<EShopCategoryType, FText> ShopCategoryNameMap;
	
private:
	//실제 객체 대상은 UObject가 아니라서 엔진에 등록하는 수명관리 불필요
	FShopState* CachedShopState;
	UPROPERTY()
	TArray<UShopItemUI*> ShopItemUIs;
	UPROPERTY()
	UShopUI* ShopUI;
	
	UPROPERTY()
	UShopCategoryBtn* CurSelectedBtn;
	UPROPERTY()
	EShopCategoryType CurSelectedCategory;

private:
	void UpdateItemList(EShopCategoryType CategoryType, UShopCategoryBtn* NewSelectBtn);
	const TArray<FShopItemEntry>* GetShopItems(EShopCategoryType CategoryType);
	void AddNewShopItemUI();
};
