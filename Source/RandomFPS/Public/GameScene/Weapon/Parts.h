// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Parts.generated.h"

UCLASS()
class RANDOMFPS_API AParts : public AActor
{
	GENERATED_BODY()
	
public:	
	FORCEINLINE UStaticMeshComponent* GetPartsMesh() const { return StaticMeshComponent; }
	AParts();

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
};
