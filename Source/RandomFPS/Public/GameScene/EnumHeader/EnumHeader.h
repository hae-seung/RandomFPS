// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWorldName : uint8
{
	Lobby,
	MainMap
};

UENUM(BlueprintType)
enum class ECardType : uint8
{
	Stat		UMETA(DisplayName = "Stat"), 
	Rifle		UMETA(DisplayName = "Rifle"),
	Shotgun		UMETA(DisplayName = "Shotgun"),
	Sniper		UMETA(DisplayName = "Sniper"),
};


UENUM(BlueprintType)
enum class ECardClass : uint8
{
	Normal		UMETA(DisplayName="Normal"),
	Silver		UMETA(DisplayName="Silver"),
	Gold		UMETA(DisplayName="Gold"),
	Platinum	UMETA(DisplayName="Platinum"),
};

UENUM()
enum class ECardTrigger : uint8
{
	OnHit,
	OnHeadshot,
	OnKill,
	OnMove
};

UENUM()
enum class EStat : uint8
{
	MaxHp,
	Hp
};

UENUM()
enum class EGun : uint8
{
	None,
	Rifle,
	Shotgun,
	Sniper
};

UENUM(BlueprintType)
enum class EPartsType : uint8
{
	None,
	Rail,
	Muzzle,
	Magazine
};

UENUM(BlueprintType)
enum class ESurfaceType : uint8
{
	Metal,
	Concrete,
	Wood
};

UENUM(BlueprintType)
enum class EEntityType : uint8
{
	Player,
	Monster,
	Boss
};