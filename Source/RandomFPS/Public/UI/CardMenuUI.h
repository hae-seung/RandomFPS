// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardMenuUI.generated.h"

class UCardSystem;
class UCardUI;
class UButton;


UCLASS()
class RANDOMFPS_API UCardMenuUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UCardSystem* InCardSystem);


protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UCardUI* CardSet1;
	UPROPERTY(meta=(BindWidget))
	UCardUI* CardSet2;
	UPROPERTY(meta=(BindWidget))
	UCardUI* CardSet3;

	UPROPERTY()
	TArray<UCardUI*> CardSets;
	
private:
	UPROPERTY()
	UCardSystem* CardSystem;



private:
	void OpenCardMenu();
	void SelectComplete(FName ID);
};
