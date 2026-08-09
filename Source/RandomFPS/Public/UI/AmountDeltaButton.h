// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "AmountDeltaButton.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCustomButtonReleased, int);

UCLASS()
class RANDOMFPS_API UAmountDeltaButton : public UButton
{
	GENERATED_BODY()
	
public:
	FOnCustomButtonReleased OnCustomButtonReleased;

public:
	void InitFirstOpen();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Amount", meta=(AllowPrivateAccess))
	int AmountDelta = 0;

private:
	UFUNCTION()
	void HandleRelease();
};
