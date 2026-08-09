// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionUI.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UInteractionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show(FText Msg);
	void Hide();

protected:
	virtual void NativeOnInitialized() override;

	
private:
	UPROPERTY()
	UImage* InteractionImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* InteractionMsgText;
	
};
