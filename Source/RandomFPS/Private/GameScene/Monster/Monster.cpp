
#include "GameScene/Monster/Monster.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CombatSystem->Init(Data);
}

void AMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	MonsterController = Cast<AMonsterController>(NewController);
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

