// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/PlayerWalletSystem.h"

#include "Net/UnrealNetwork.h"

UPlayerWalletSystem::UPlayerWalletSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UPlayerWalletSystem::BeginPlay()
{
	Super::BeginPlay();

	OnMoneyChanged.Broadcast(Money);
}

void UPlayerWalletSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerWalletSystem, Money);
}

int UPlayerWalletSystem::GetMoney() const
{
	return Money;
}

void UPlayerWalletSystem::SpendMoney(int OutAmount)
{
	Money -= OutAmount;
	OnMoneyChanged.Broadcast(Money);
}

void UPlayerWalletSystem::AcquireMoney(int InAmount)
{
	Money += InAmount;
	OnMoneyChanged.Broadcast(Money);
}

void UPlayerWalletSystem::OnRep_Money()
{
	OnMoneyChanged.Broadcast(Money);
}
