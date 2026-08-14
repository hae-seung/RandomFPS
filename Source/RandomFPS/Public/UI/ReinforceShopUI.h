// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldInteractionUI.h"
#include "Components/HorizontalBox.h"
#include "ReinforceShopUI.generated.h"


class UGunAbilityDescriptionUI;
class UGunItem;
class AReinforceShop;
class UReinforceStarUI;
class UHorizontalBox;
class UVerticalBox;
class UImage;
class UTextBlock;
class USizeBox;
class UButton;
class UOverlay;

UCLASS()
class RANDOMFPS_API UReinforceShopUI : public UWorldInteractionUI
{
	GENERATED_BODY()

public:
	void FirstOpenInit(AReinforceShop* RS, APlayerCharacter* InAPC);
	virtual void Open() override;

	void FailToReinforce();
	void FailToReinforce(int RemainMoney);
	void SuccessAwakeGun(int GunLevel, int RemainMoney);
	void SuccessReinforceGun(int GunLevel, int RemainMoney);
	

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta=(BindWidget))
	UOverlay* NoGunWarningTab;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* UserMoneyText;
	UPROPERTY(meta=(BindWidget))
	UButton* ExitBtn;
	
	UPROPERTY(meta=(BindWidget))
	UOverlay* ReinforceTab;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* PGunInfo;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PGunNameText;
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* PStarBox;
	UPROPERTY(meta=(BindWidget))
	UImage* PGunImage;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* PGunAbilityBox;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* NextLevelInfoBox;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* UpgradeNeedMoneyText;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* NGunInfo;
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* NStarBox;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NGunNameText;
	UPROPERTY(meta=(BindWidget))
	UImage* NGunImage;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* NGunAbilityBox;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* SuccessProbText;
	UPROPERTY(meta=(BindWidget))
	UButton* CancelBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* ReinforceBtn;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UReinforceStarUI> WB_ReinforceStarUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGunAbilityDescriptionUI> WB_DescriptionUI;
	UPROPERTY(EditAnywhere)
	float DescriptionFontSize;
	
private:
	bool bIsSyncComplete;
	
	UPROPERTY()
	AReinforceShop* Client_RS;
	UPROPERTY()
	UGunItem* Client_Gun;
	
	UPROPERTY()
	TArray<UReinforceStarUI*> PStars;
	UPROPERTY()
	TArray<UReinforceStarUI*> NStars;
	UPROPERTY()
	TArray<UGunAbilityDescriptionUI*> PGunAbilityTexts;
	UPROPERTY()
	TArray<UGunAbilityDescriptionUI*> NGunAbilityTexts;

private:
	UFUNCTION()
	void CloseUI();
	UFUNCTION()
	void RequestReinforce();
	
	void CreateInitStars(TArray<UReinforceStarUI*>& Stars, UHorizontalBox* Box);
	void SetGunReinforceStateUI(int CurLevel);
	void SetGunAwakeStateUI(int CurLevel);
	void SetGunAbilityTexts(
		int CurLevel,
		TArray<UGunAbilityDescriptionUI*>& GunAbilityTexts,
		UVerticalBox* AbilityBox);
	void AddGunAwakeAbilityText();
	
	void SetGunInfoPrev(int CurLevel);
	void SetGunInfoNext(int NextLevel);
	void SetStar(int Level, TArray<UReinforceStarUI*>& StarBox);
	void SetSync(bool bState);
};
