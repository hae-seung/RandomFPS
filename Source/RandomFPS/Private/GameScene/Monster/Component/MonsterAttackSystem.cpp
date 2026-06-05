// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Monster/Component/MonsterAttackSystem.h"

#include "GameScene/Monster/Monster.h"

UMonsterAttackSystem::UMonsterAttackSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMonsterAttackSystem::BeginPlay()
{
	Super::BeginPlay();

	if(AMonster* Owner = Cast<AMonster>(GetOwner()))
	{
		Monster = Owner;
	}
}


void UMonsterAttackSystem::StartAttack()
{
	Def = Monster->GetCurAttackDefinition();
	if(!Def)
		return;

	FVector SocketLoc = Monster->GetMesh()->GetSocketLocation(Def->SocketName);

	AttackStartLocation =
		SocketLoc + GetOwner()->GetActorForwardVector() * Def->ForwardOffset;

	FVector WorldDir =
		Monster->GetActorRotation().RotateVector(Def->LocalDirection);

	WorldDir.Normalize();

	AttackEndLocation = AttackStartLocation + WorldDir * Def->Range;

	Shape = FCollisionShape::MakeSphere(Def->Radius);
}

void UMonsterAttackSystem::EndAttack()
{
	TraceAttack(); //실제 공격 판정 실행
}

void UMonsterAttackSystem::TraceAttack()
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Monster);

	FHitResult Hit;

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		AttackStartLocation,
		AttackEndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Shape,
		Params
	);

	if(bHit)
	{
		Monster->ApplyDamage(Hit);
	}

	// =========================
	// DEBUG VISUALIZATION
	// =========================

	// 1. Start / End line (핵심 경로)
	DrawDebugLine(
		GetWorld(),
		AttackStartLocation,
		AttackEndLocation,
		FColor::Red,
		false,
		2.0f,
		0,
		2.0f
	);

	// 2. Start sphere
	DrawDebugSphere(
		GetWorld(),
		AttackStartLocation,
		Def->Radius,
		12,
		FColor::Green,
		false,
		2.0f
	);

	// 3. End sphere
	DrawDebugSphere(
		GetWorld(),
		AttackEndLocation,
		Def->Radius,
		12,
		FColor::Blue,
		false,
		2.0f
	);

	// 4. Hit point
	if (bHit)
	{
		DrawDebugPoint(
			GetWorld(),
			Hit.ImpactPoint,
			10.0f,
			FColor::Yellow,
			false,
			2.0f
		);

		DrawDebugString(
			GetWorld(),
			Hit.ImpactPoint,
			Hit.GetActor() ? Hit.GetActor()->GetName() : TEXT("Hit"),
			nullptr,
			FColor::White,
			2.0f,
			true
		);
	}
}