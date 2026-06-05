// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "PartsSlotUI.generated.h"

class UImage;

DECLARE_DELEGATE_OneParam(FRightMouseButtonClickedEvent, EPartsType);

UCLASS()
class RANDOMFPS_API UPartsSlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FRightMouseButtonClickedEvent RightMouseButtonClickedEvent;
	
public:
	void Init();
	void BindRightMouseButtonEvent(FRightMouseButtonClickedEvent ClickEvent);
	
	FORCEINLINE bool CheckCanSlotUsable() const { return bCanUse; }
	
	void SetSlotUsable(bool value);
	void UpdateImage(UTexture2D* Image);
	void SetHighlight();
	void OffHighlight();
	
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UImage* PartsImage;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UImage* HighlightImage;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	EPartsType SlotPartsType;
	
	bool bCanUse = true;
	bool bHasParts = false;

private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
