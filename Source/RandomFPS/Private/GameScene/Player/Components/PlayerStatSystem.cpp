// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/Components/PlayerStatSystem.h"

#include "GameScene/Player/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

UPlayerStatSystem::UPlayerStatSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	EnergyIndex = -1;
}

void UPlayerStatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerStatSystem, HealthStat);
	DOREPLIFETIME(UPlayerStatSystem, CombatStat);
	DOREPLIFETIME(UPlayerStatSystem, UtilityStat);
	
	DOREPLIFETIME(UPlayerStatSystem, EnergyIndex);
}


void UPlayerStatSystem::BeginPlay()
{
	Super::BeginPlay();
	
	OriginWalkSpeed = UtilityStat.WalkSpeed;
	
	if(GetOwner()->HasAuthority())
	{
		HealthStat.Hp = HealthStat.MaxHp;
	}
	
	InitDelegates();

	APC = Cast<APlayerCharacter>(GetOwner());
	APC->SetPlayerSpeed(UtilityStat.WalkSpeed);
}

void UPlayerStatSystem::InitDelegates() const
{
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
	OnPlayerCombatStatChanged.Broadcast(CombatStat);
	OnPlayerUtilityStatChanged.Broadcast(UtilityStat);

	OnPlayerEnergyChanged.Broadcast(EnergyIndex);
}

void UPlayerStatSystem::Revive()
{
	HealthStat.Hp = HealthStat.MaxHp;
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}


void UPlayerStatSystem::ModifyHp(float Delta)
{
	HealthStat.Hp = FMath::Clamp(HealthStat.Hp + Delta, 0, HealthStat.MaxHp);

	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}

void UPlayerStatSystem::ApplyPortion(const FStatModifier& Modifier)
{
	switch (Modifier.Stat)
	{
	case EStat::Energy:
		HandleEnergy(Modifier.Value);
		break;
	case EStat::Hp:
		HandleHp(Modifier.Value);
		break;
	case EStat::MaxHp:
		HandleMaxHp(Modifier.Value);
		break;
	case EStat::WalkSpeed:
		HandleWalkSpeed(Modifier.Value);
		break;
	}
}

void UPlayerStatSystem::HandleEnergy(float Value)
{
	const int OriginIndex = EnergyIndex;
	EnergyIndex = FMath::Clamp(EnergyIndex + Value, -1, 9);
	ApplyEnergyWalkSpeed();
	
	if(OriginIndex < 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			EnergyTimer,
			this, &UPlayerStatSystem::EnergyTick,
			DrinkApplyWaitTime,
			false);
	}

	//UI갱신
	OnPlayerEnergyChanged.Broadcast(EnergyIndex);
}

void UPlayerStatSystem::HandleHp(float Value)
{
	HealthStat.Hp = FMath::Clamp(HealthStat.Hp + Value, 0 , HealthStat.MaxHp);
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}

void UPlayerStatSystem::HandleMaxHp(float Value)
{
	HealthStat.MaxHp += Value;
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}

void UPlayerStatSystem::HandleWalkSpeed(float Value)
{
	UtilityStat.WalkSpeed = FMath::Max(UtilityStat.WalkSpeed + Value, 100.f);
	OnPlayerUtilityStatChanged.Broadcast(UtilityStat);
	APC->SetPlayerSpeed(UtilityStat.WalkSpeed);
}

void UPlayerStatSystem::EnergyTick()
{
	//드링크 능력 적용
	const float HealValue = HealthStat.MaxHp * (DrinkStats[EnergyIndex].RecoveryPercent * 0.01f);
	HandleHp(HealValue);

	
	EnergyIndex--;
	OnPlayerEnergyChanged.Broadcast(EnergyIndex);
	
	if(EnergyIndex >= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			EnergyTimer,
			this, &UPlayerStatSystem::EnergyTick,
			DrinkApplyWaitTime,
			false);

		ApplyEnergyWalkSpeed();
	}
}

void UPlayerStatSystem::ApplyEnergyWalkSpeed()
{
	//드링크의 이동속도 %는 기본 걷기 속도에서 계산되어야 한다.
	const float TargetSpeed =
		OriginWalkSpeed + (OriginWalkSpeed * (DrinkStats[EnergyIndex].WalkSpeedAddPercent * 0.01f));
	
	HandleWalkSpeed(TargetSpeed - UtilityStat.WalkSpeed);
}






void UPlayerStatSystem::OnRep_HealthStat()
{
	OnPlayerHealthStatChanged.Broadcast(HealthStat);
}

void UPlayerStatSystem::OnRep_CombatStat()
{
	
}

void UPlayerStatSystem::OnRep_UtilityStat()
{
	OnPlayerUtilityStatChanged.Broadcast(UtilityStat);
}

void UPlayerStatSystem::OnRep_EnergyIndex()
{
	//UI갱신
	OnPlayerEnergyChanged.Broadcast(EnergyIndex);
}

