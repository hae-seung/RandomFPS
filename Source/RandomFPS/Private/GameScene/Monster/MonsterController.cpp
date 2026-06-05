// Fill out your copyright notice in the Description page of Project Settings.

#include "GameScene/Monster/MonsterController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameScene/Monster/Monster.h"
#include "GameScene/Monster/MonsterData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AMonsterController::AMonsterController()
{
	SetGenericTeamId(FGenericTeamId(1));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerceptionComponent);	//컨트롤러에 등록
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(
		this, &AMonsterController::TargetPerceptionUpdated);
}


void AMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(!HasAuthority())
	{
		if(AIPerceptionComponent)
		{
			AIPerceptionComponent->DestroyComponent();
			AIPerceptionComponent = nullptr;
		}
		return;
	}
	
	
	Monster = Cast<AMonster>(InPawn);
	if(IsValid(Monster))
	{
		UBehaviorTree* BT = Monster->GetBT();
		if(BT)
		{
			MonsterData = Monster->GetData();
			SetMonsterPerception(MonsterData);
			
			RunBehaviorTree(BT);
			BB = GetBlackboardComponent();
		}
	}
}


void AMonsterController::SetMonsterPerception(UMonsterData* Data)
{
	//데이터를 통해 무슨 인지를 가졌는지 판단 후 각 인지에 맞게 세팅.
	if(Data->IsUseSight())
	{
		InitSightPerception(Data->GetSightInfo());
	}
	else
	{
		//사용하지 않는 센스는 끄기
		AIPerceptionComponent->SetSenseEnabled(SightConfig->GetSenseImplementation(), false);
	}

	//hearing도 추가하게 되면 Data에서 Enum을 통해 누가 주센서인지 체크 후
	//해당 센서로 업데이트
	//todo: SetDominantSense(Data->GetHighPrioritySense())
	SetDominantSense();

	
	//인지데이터가 변경되었으므로 리스너 업데이트를 위해 Sense(계산기)에게 알려줘야함.
	//->새로운 리스너 생성 후 등록됨.
	AIPerceptionComponent->RequestStimuliListenerUpdate();
}

void AMonsterController::InitSightPerception(const FAISightPerceptionInfo& SightInfo)
{
	SightConfig = NewObject<UAISenseConfig_Sight>(this);
	AIPerceptionComponent->ConfigureSense(*SightConfig);
    	
	//SightConfig
	SightConfig->SightRadius = SightInfo.SightRadius;
	SightConfig->LoseSightRadius = SightInfo.LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = SightInfo.SightAngle;
	SightConfig->SetMaxAge(3.f);

	//Detect
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	//벽 뒤에 숨었는데 자동감지 방지
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f; 
}

void AMonsterController::SetDominantSense()
{
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AMonsterController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	const FAISenseID SenseId = Stimulus.Type;
	
	if(SenseId == UAISense::GetSenseID<UAISense_Sight>())
	{
		if(Stimulus.WasSuccessfullySensed())
		{
			//누군가가 감지됨
			ActorInSight(Actor);
		}
		else
		{
			//기존 감지대상이 없어짐
			ActorOutSight(Actor);
		}
	}
}

void AMonsterController::ActorInSight(AActor* Actor)
{
	if(!IsValid(CurrentTarget))
	{
		ChangeCurrentTarget(Actor);
	}
	
	Targets.Add(Actor);
}

void AMonsterController::ActorOutSight(AActor* Actor)
{
	Targets.Remove(Actor);

	//인지 벗어난 사람이 현재 타겟이었던 경우
	if(CurrentTarget == Actor)
	{
		ChangeCurrentTarget(GetTargetInTargets());
	}
}

void AMonsterController::ChangeCurrentTarget(AActor* NewTargetActor)
{
	if(CurrentTarget == NewTargetActor)
		return;

	//타겟을 잃음
	if(IsValid(CurrentTarget) && NewTargetActor == nullptr)
	{
		BB->SetValueAsVector(LostLocationKey, CurrentTarget->GetActorLocation());
		BB->SetValueAsBool(LostTargetKey, true);
		BB->SetValueAsObject(TargetActorKey, nullptr);
		
		Targets.Remove(CurrentTarget);
		CurrentTarget = nullptr;
		return;
	}

	//타겟 획득 또는 변경
	CurrentTarget = NewTargetActor;
	Targets.Add(CurrentTarget);
	BB->SetValueAsObject(TargetActorKey, CurrentTarget);
	BB->SetValueAsBool(LostTargetKey, false);
}

AActor* AMonsterController::GetTargetInTargets()
{
	//거리가 가장 가까운 플레이어를 타겟으로 설정
	if(Targets.Num() <= 0)
		return nullptr;

	AActor* NewTarget = nullptr;
	float SmallestDistance = TNumericLimits<float>::Max();
	for(AActor* Actor : Targets)
	{
		const float TempDistance = FVector::DistSquared(
			GetPawn()->GetActorLocation(),
			Actor->GetActorLocation());

		if(SmallestDistance > TempDistance)
		{
			SmallestDistance = TempDistance;
			NewTarget = Actor;
		}
	}

	return NewTarget;
}

void AMonsterController::CheckCanAttackTarget()
{
	if(!IsValid(CurrentTarget) || bIsAttacking)
	{
		//공격판정이 계속 가능하다고 착각하는 오판 방지
		BB->SetValueAsBool(CanAttackKey, false);
		return;
	}
	
	const float AttackRangeSq = FMath::Square(MonsterData->GetAttackRange());
	const bool bCanAttack = AttackRangeSq >= FVector::DistSquared(
		CurrentTarget->GetActorLocation(), Monster->GetActorLocation());
	
	BB->SetValueAsBool(CanAttackKey, bCanAttack);
}

//움찔 확정 났을 때 호출됨
void AMonsterController::SetFlinchState(AActor* Attacker) 
{
	if(!IsValid(CurrentTarget))
	{
		ChangeCurrentTarget(Attacker);
	}
	else
	{
		ChangeCurrentTarget(GetTargetInTargets());
	}
	
	if(!bIsFlinching)
	{
		bIsFlinching = true;
		BB->SetValueAsBool(FlinchKey, true);
		PlayFlinchMontage();
	}
}

void AMonsterController::PlayFlinchMontage()
{
	//monster에 Flinch몽타주 실행
	UAnimMontage* FlinchMontage = MonsterData->GetFlinchMontage();
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMonsterController::OnFlinchEnd);
	
	Monster->PlayMontageAndSubscribeEndDelegate(FlinchMontage, EndDelegate);
}

void AMonsterController::Attack()
{
	bIsAttacking = true;

	//monster에 몽타주 실행
	UAnimMontage* AttackMontage = MonsterData->GetAttackDefinition().AttackMontage;
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMonsterController::OnAttackEnd);

	Monster->SetCurrentAttackDefinition(MonsterData->GetAttackDefinition());
	Monster->PlayMontageAndSubscribeEndDelegate(AttackMontage, EndDelegate);
}

void AMonsterController::OnAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
}

void AMonsterController::OnFlinchEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsFlinching = false;
	BB->SetValueAsBool(FlinchKey, false);
}

void AMonsterController::OnDeadEnd(UAnimMontage* Montage, bool bInterrupted)
{
	
}
