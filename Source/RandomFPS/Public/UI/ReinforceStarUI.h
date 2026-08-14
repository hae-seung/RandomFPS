// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReinforceStarUI.generated.h"

class USizeBox;
class UImage;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UReinforceStarUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	void SetReinforce();
	void SetEmpty();
	void AwakeStar();
	void SetStarSize(float X, float Y);
	
private:
	UPROPERTY(meta=(BindWidget))
	USizeBox* StarSize;
	
	UPROPERTY(meta=(BindWidget))
	UImage* StarImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* EmptyStarImage;
	UPROPERTY(EditAnywhere)
	UTexture2D* ReinforceStarImage;
	UPROPERTY(EditAnywhere)
	UTexture2D* AwakeStarImage;
};
