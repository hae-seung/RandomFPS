// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "CheatButton.generated.h"


class UItemData;


DECLARE_DELEGATE_OneParam(FOnCheatBtnClicked, UItemData*);


UCLASS()
class RANDOMFPS_API UCheatButton : public UButton
{
	GENERATED_BODY()

public:
	FOnCheatBtnClicked OnCheatBtnClicked;

public:
	void Init();

private:
	UPROPERTY(EditAnywhere)
	UItemData* ItemData;

private:
	UFUNCTION()
	void CheatBtnClicked();
};
