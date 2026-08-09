// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/ShopStruct.h"
#include "ShopItemUI.generated.h"

class UButton;
class UImage;
class UTextBlock;

DECLARE_DELEGATE_TwoParams(FOnBuyBtnClicked, const FShopItemEntry*, EShopCategoryType);

UCLASS()
class RANDOMFPS_API UShopItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnBuyBtnClicked OnBuyBtnClicked;

public:
	void SetShopItemInfo(const FShopItemEntry& ShopItemEntry, EShopCategoryType InItemCategory);

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameText;
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StockText;
	UPROPERTY(meta=(BindWidget))
	UButton* BuyBtn;

private:
	//구조체 포인터는 GC의 참조대상이 아니다.
	//언리얼이 추적하여 생명주기를 보장하지 않아도 된다.
	//구조체는 "값"으로서 이미 누군가가 소유하고 있다.
	//따라서 포인터로 해당 구조체를 가리킬땐 GC에 등록할 필요가 없다.
	//소유주가 파괴되면 해당 구조체도 알아서 같이 파괴되기 때문이다.
	const FShopItemEntry* ItemEntry;
	EShopCategoryType ItemCategory;
	
private:
	UFUNCTION()
	void OpenBuyQuantityTab();
};
