// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardSlotWrapperUI.generated.h"

class UCardSlotUI;
class UCardData;
class UCardSystem;
class UInventoryUI;


UCLASS()
class RANDOMFPS_API UCardSlotWrapperUI : public UUserWidget
{
	GENERATED_BODY()


public:
	void Init(UInventoryUI* InInventoryUI, UCardSystem* InCardSystem);


protected:
	virtual void NativeOnInitialized() override;
	

private:
	UPROPERTY(meta=(BindWidget))
	UCardSlotUI* CardSlot1;
	UPROPERTY(meta=(BindWidget))
	UCardSlotUI* CardSlot2;
	UPROPERTY(meta=(BindWidget))
	UCardSlotUI* CardSlot3;
	UPROPERTY(meta=(BindWidget))
	UCardSlotUI* CardSlot4;
	
	
private:
	UPROPERTY()
	UInventoryUI* InventoryUI;
	UPROPERTY()
	UCardSystem* CardSystem;

	UPROPERTY()
	TArray<UCardSlotUI*> CardSlots;
	
	int FillCardIndex;

private:
	void SetCardSlot(UCardData* NewCardData);
};
