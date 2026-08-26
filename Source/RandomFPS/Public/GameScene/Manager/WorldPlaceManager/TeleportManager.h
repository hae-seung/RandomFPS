// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "TeleportManager.generated.h"

class ATargetPoint;

UCLASS()
class RANDOMFPS_API ATeleportManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATeleportManager();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


private:
	UPROPERTY(EditAnywhere)
	ATargetPoint* RelaxPoint;
	UPROPERTY(EditAnywhere)
	TArray<ATargetPoint*> PvePoints;
	UPROPERTY(EditAnywhere)
	TArray<ATargetPoint*> PvpPoints;

private:
	void TeleportRelaxRoom();
	void TeleportRoundArea(ERoundType RoundType);
	void ShufflePoints(TArray<ATargetPoint*>& Points);
	void TeleportToArea(TArray<ATargetPoint*>& Points);
};
