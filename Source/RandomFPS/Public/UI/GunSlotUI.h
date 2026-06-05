// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GunSlotUI.generated.h"


class UPartsItem;
class UInventoryUI;
class UImage;
class UPartsSlotUI;
class UGunItem;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UGunSlotUI : public UUserWidget
{
	GENERATED_BODY()


public:
	void Init(UInventoryUI* InvenUI);
	void UpdateNewGun(UGunItem* NewGun);
	void UpdateNewParts(EPartsType PartsType, UTexture2D* Icon = nullptr);
	void SetGunRenderTarget(UTextureRenderTarget2D* RT);
	
private:
	UPROPERTY(meta=(BindWidget))
	UPartsSlotUI* RailPartsSlot;
	UPROPERTY(meta=(BindWidget))
	UPartsSlotUI* MuzzlePartsSlot;
	UPROPERTY(meta=(BindWidget))
	UPartsSlotUI* MagazinePartsSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UImage* HighlightImage;

	UPROPERTY(meta=(BindWidget, AllowPrivateAccess))
	UImage* GunPreviewImage;
	UPROPERTY(EditAnywhere)
	UMaterial* BaseMat;

	UInventoryUI* InventoryUI;
	UPartsSlotUI* CurrentHighlightPartsSlotUI;

	
	TMap<EPartsType, UPartsSlotUI*> PartsSlotMap;

private:
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void OnPartsSlotMouseRightButtonClicked(EPartsType PartsType);
};
