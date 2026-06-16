// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/Component/MonsterCombatSystem.h"

#include "GameScene/Monster/MonsterData.h"
#include "Interface/Damageable.h"
#include "Net/UnrealNetwork.h"

UMonsterCombatSystem::UMonsterCombatSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UMonsterCombatSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UMonsterCombatSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMonsterCombatSystem, MaxHp);
	DOREPLIFETIME(UMonsterCombatSystem, Hp);
}

void UMonsterCombatSystem::Init(UMonsterData* Data)
{
	Stat = Data->GetMonsterStatData();
	
	Hp = Stat.MaxHP;
	MaxHp = Stat.MaxHP;
	OnMonsterHealthStatChanged.Broadcast(Hp, MaxHp);
}

void UMonsterCombatSystem::ApplyDamage(FHitResult& HitResult)
{
	//몬스터 기준 맞은 놈은 무조건 플레이어임.(Trace로 걸러낸 상태임)
	if(IDamageable* DamageableActor = Cast<IDamageable>(HitResult.GetActor()))
	{
		FDamageContext AttackContext;
		AttackContext.EntityType = Stat.EntityType;
		AttackContext.Attacker = GetOwner();
		AttackContext.BaseDamage = Stat.AttackDamage;
		AttackContext.FinalDamage = CalculateAttackDamage(AttackContext);

		DamageableActor->TakeDamage(AttackContext);
	}
}


float UMonsterCombatSystem::CalculateAttackDamage(FDamageContext& AttackContext)
{
	bool IsCritical = FMath::RandRange(1, 100) <= Stat.CriticalProbability;
	AttackContext.bIsCritical = IsCritical;
	
	float Damage = IsCritical ? Stat.AttackDamage * 2 : Stat.AttackDamage;
	const float RandNum = Damage * Stat.RandomAttackDamageRange;
	Damage = FMath::RandRange(Damage - RandNum, Damage + RandNum);
	
	return  Damage;
}

void UMonsterCombatSystem::OnRep_Hp()
{
	OnMonsterHealthStatChanged.Broadcast(Hp, MaxHp);
}

void UMonsterCombatSystem::OnRep_MaxHp()
{
	OnMonsterHealthStatChanged.Broadcast(Hp, MaxHp);
}
