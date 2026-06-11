// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/Cards/StructHeader.h"
#include "UIManager.generated.h"

class UBulletItemData;
class UCombatUI;
class UPartsItemData;
class UGunSlotUI;
class URailPartsData;
class UButton;
class UInventoryUI;
class UCanvasPanel;
class UCrossHairUI;
class AMyPlayerController;
class UGunMenu;


UCLASS()
class RANDOMFPS_API UUIManager : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UPartsItemData* RailData;
	UPROPERTY(EditAnywhere)
	FBulletService BulletService;
	
	
public:
	void ToggleInventory();

	UPROPERTY(meta=(BindWidget))
	UButton* RedDotBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* BulletBtn;
	
	UFUNCTION()
	void GiveRedDot();
	UFUNCTION()
	void GiveBullet();

	FORCEINLINE UInventoryUI* GetInventoryUI()const { return InventoryUI; }
	UCombatUI* GetCombatUI() const { return CombatUI; }
	void Init(APawn* Pawn);
	
private:
	UPROPERTY()
	AMyPlayerController* LocalController;

	//CanvasPanel자식으로 UISet을 두어야함.
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget,AllowPrivateAccess))
	UInventoryUI* InventoryUI;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget,AllowPrivateAccess))
	UGunMenu* GunMenuUI;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget,AllowPrivateAccess))
	UCrossHairUI* CrossHairUI;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UCombatUI* CombatUI;
};
