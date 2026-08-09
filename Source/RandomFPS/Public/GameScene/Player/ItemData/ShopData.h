// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Struct/ShopStruct.h"
#include "ShopData.generated.h"



UCLASS()
class RANDOMFPS_API UShopData : public UDataAsset
{
	GENERATED_BODY()

public:
	const FShopState& GetShopData() const;
	
private:
	UPROPERTY(EditAnywhere)
	FShopState ShopData;
};
