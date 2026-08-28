// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Cards/CardAbility/Base/MonsterKillerAbility.h"

#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerStatSystem.h"

UCardAbility* UMonsterKillerAbility::Clone()
{
	return NewObject<UMonsterKillerAbility>(this);
}

void UMonsterKillerAbility::OnAcquire(APlayerCharacter* Player)
{
	PlayerStatSystem = Player->GetStatComponent();
	Player->KillMonsterEvent.AddUObject(this, &UMonsterKillerAbility::KillMonster);
}

void UMonsterKillerAbility::OnRemove(APlayerCharacter* Player)
{
	
}

void UMonsterKillerAbility::KillMonster()
{
	PlayerStatSystem->ApplyStatModifier(PlusMaxHp);
}
