// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemUseTask.generated.h"

class UPlayerStatSystem;
class UPlayerInteractSystem;
class UItemInstance;
class APlayerCharacter;
class IUseable;

DECLARE_DELEGATE_OneParam(FOnUseItemComplete, int32);

UCLASS()
class RANDOMFPS_API UItemUseTask : public UObject
{
	GENERATED_BODY()

public:
	FOnUseItemComplete OnUseItemComplete;

public:
	void Init(UPlayerInteractSystem* UserInteractSystem,
		UPlayerStatSystem* PlayerStatSystem);
	
	void RegisterItem(UItemInstance* UsableItem, int idx);
	void StopUseItem();
	void StartUseItem(UItemInstance* Item);

	
private:
	int Index = -1;
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	UPlayerInteractSystem* InteractSystem;
	UPROPERTY()
	UPlayerStatSystem* StatSystem;
	UPROPERTY()
	UItemInstance* BookedItem;

private:
	void RealUseItem();
};
