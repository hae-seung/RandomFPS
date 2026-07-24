// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractorUI.generated.h"

class UPlayerInteractSystem;
class UTextBlock;
class UImage;
class UOverlay;


UCLASS()
class RANDOMFPS_API UInteractorUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UPlayerInteractSystem* InteractSystem);


	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta=(BindWidget))
	UImage* ProgressBarImage;
	UPROPERTY(meta=(BindWidget))
	UImage* InteractingImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RemainText;

	UPROPERTY()
	UMaterialInstanceDynamic* ProgressMID;

	bool bInteracting;
	float ServerEndTime;
	float TotalTime;

private:
	void Show();
	void Hide();

	void UpdateInteractUI(float EndTime, float InteractTotalTime, UTexture2D* Icon);
	void UpdateProgressTick();
};
