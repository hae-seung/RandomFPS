// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WalletUI.generated.h"

class UTextBlock;
class UPlayerWalletSystem;



UCLASS()
class RANDOMFPS_API UWalletUI : public UUserWidget
{
	GENERATED_BODY()


public:
	void Init(UPlayerWalletSystem* WalletSystem);

private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GoldText;

private:
	void ChangeGoldText(int NewGoldAMount);
};
