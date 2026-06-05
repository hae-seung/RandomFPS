// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragSlot.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API UDragSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetImage(UTexture2D* IconImage);

private:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
};
