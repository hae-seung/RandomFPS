
#include "GameScene/Monster/Monster.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameScene/PoolManager.h"
#include "GameScene/Monster/DamageActor.h"
#include "GameScene/Monster/MonsterController.h"
#include "GameScene/Monster/MonsterData.h"
#include "GameScene/Monster/Component/MonsterAttackSystem.h"
#include "GameScene/Monster/Component/MonsterCombatSystem.h"
#include "UI/HealthUI.h"


AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CombatSystem = CreateDefaultSubobject<UMonsterCombatSystem>(TEXT("MonsterCombatSystem"));
	AttackSystem = CreateDefaultSubobject<UMonsterAttackSystem>(TEXT("MonsterAttackSystem"));
	HealthBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarUI"));
	HealthBarComp->SetupAttachment(GetRootComponent());
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CombatSystem->Init(Data);
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	UHealthUI* HealthUI = Cast<UHealthUI>(HealthBarComp->GetUserWidgetObject());
	if(HealthUI)
	{
		CombatSystem->OnMonsterHealthStatChanged.AddUObject(HealthUI, &UHealthUI::UpdateHealth);
		HealthUI->Init(HealthBarComp);
	}
}

void AMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	MonsterController = Cast<AMonsterController>(NewController);
	CombatSystem->OnMonsterFlinched.AddUObject(MonsterController, &AMonsterController::SetFlinchState);
	CombatSystem->OnMonsterDead.AddUObject(MonsterController, &AMonsterController::SetDeadState);
}

UBehaviorTree* AMonster::GetBT() const
{
	return BT;
}

void AMonster::SetCurrentAttackDefinition(const FMonsterAttackDefinition& AttackDefinition)
{
	CurrentAttackDefinition = &AttackDefinition;
}

void AMonster::Multicast_PlayMontage_Implementation(UAnimMontage* Montage)
{
	if(HasAuthority())
		return;
	
	PlayAnimMontage(Montage);
}

void AMonster::PlayMontageAndSubscribeEndDelegate(UAnimMontage* Montage, FOnMontageEnded& EndDelegate)
{
	if(!HasAuthority())
		return;
	
	PlayAnimMontage(Montage);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, Montage);
	Multicast_PlayMontage(Montage);
}

void AMonster::StartAttack()
{
	if(!HasAuthority())
		return;
	
	AttackSystem->StartAttack();
}

void AMonster::EndAttack()
{
	if(!HasAuthority())
		return;
	
	AttackSystem->EndAttack();
}

EEntityType AMonster::GetEntityType()
{
	return Data->GetEntityType();
}

void AMonster::TakeDamage(FDamageContext& Context)
{
	if(!HasAuthority())
		return;
	
	float TakenDamage = CombatSystem->TakeDamage(Context);
	MonsterController->MonsterAttackedByPlayer(Context.Attacker);
	
	//multicast effect
	Server_SpawnBlood(Context.HitLocation);
	Server_SpawnDamageActor(Context.HitLocation, TakenDamage, Context.bIsCritical);

	//3D사운드
}

void AMonster::Server_SpawnBlood(FVector& HitLocation)
{
	SpawnBlood(HitLocation);
	Multicast_SpawnBlood(HitLocation);
}


void AMonster::SpawnBlood(FVector HitLocation)
{
	UPoolManager* PoolManager = GetWorld()->GetSubsystem<UPoolManager>();
	PoolManager->Client_PlayLocationFX(
		Data->GetBloodEffect(),
		HitLocation,
		FRotator::ZeroRotator);
}

void AMonster::Multicast_SpawnBlood_Implementation(FVector HitLocation)
{
	if(HasAuthority())
		return;

	SpawnBlood(HitLocation);
}

void AMonster::Server_SpawnDamageActor(FVector& HitLocation, float Damage, bool bIsCritic)
{
	SpawnDamageActor(HitLocation, Damage, bIsCritic);
	Multicast_SpawnDamageActor(HitLocation, Damage, bIsCritic);
}

void AMonster::Multicast_SpawnDamageActor_Implementation(FVector HitLocation, float Damage, bool bIsCritic)
{
	if(HasAuthority())
		return;

	SpawnDamageActor(HitLocation, Damage, bIsCritic);
}

void AMonster::SpawnDamageActor(FVector& HitLocation, float Damage, bool bIsCritic)
{
	UPoolManager* PoolManager = GetWorld()->GetSubsystem<UPoolManager>();

	ADamageActor* DamageActor = Cast<ADamageActor>(PoolManager->Server_GetActor(
		Data->GetBP_DamageActor(),
		this));

	if(IsValid(DamageActor))
	{
		DamageActor->SetDamageText(Damage, bIsCritic);
		DamageActor->SetActorLocation(
			HitLocation + DamageActor->GetActorForwardVector() * 40.f);
	}
}



bool AMonster::GetIsDead()
{
	return bIsDead;
}


void AMonster::ApplyDamage(FHitResult& HitResult)
{
	if(!HasAuthority())
		return;
	
	CombatSystem->ApplyDamage(HitResult);
}



