// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScene/Cards/StructHeader.h"
#include "UIManager.generated.h"

class UItemCheatUI;
class UCardMenuUI;
class UWalletUI;
class URoundUI;
class UInteractorUI;
class UPortionItemData;
class UKillLogUI;
class AMyPlayerState;
class UScoreUI;
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
	void ToggleInventory();
	void ToggleCheatUI();
	FORCEINLINE UInventoryUI* GetInventoryUI()const { return InventoryUI; }
	UCombatUI* GetCombatUI() const { return CombatUI; }
	void Init(APawn* Pawn);
	void OpenScoreBoard();
	void CloseScoreBoard();
	void ToggleCombatUI(bool bOpen);
	
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
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UScoreUI* ScoreUI;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UKillLogUI* KillLogUI;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UInteractorUI* InteractorUI;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	URoundUI* RoundUI;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UWalletUI* WalletUI;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UCardMenuUI* CardMenuUI;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess))
	UItemCheatUI* CheatUI;
};
