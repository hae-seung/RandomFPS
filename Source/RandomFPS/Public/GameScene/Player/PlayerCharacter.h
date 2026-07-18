// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "GameScene/EnumHeader/EnumHeader.h"
#include "GameFramework/Character.h"
#include "Interface/Damageable.h"
#include "Interface/Killable.h"
#include "PlayerCharacter.generated.h"

class UPlayerStatSystem;
class UPlayerCombatSystem;
class URailPartsData;
class UItemData;
class UItemInstance;
class UGunItemData;
class UInventory;
class UPlayerWeapon;
class UCardManager;
struct FInputActionValue;
class UInputAction;
class AMyPlayerController;
class UCameraComponent;
class USpringArmComponent;


DECLARE_MULTICAST_DELEGATE(FOnKillMonster);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnKillAssist, AActor*);
DECLARE_MULTICAST_DELEGATE(FOnKillCountPlus); //UI 킬+1 용도
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnKillPlayer, AActor*, AActor*, bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnKillAlarm, AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAssistAlarm, AActor*);


UCLASS()
class RANDOMFPS_API APlayerCharacter :
public ACharacter,
public IGenericTeamAgentInterface,
public IDamageable,
public IKillable
{
	GENERATED_BODY()

public:
	FOnKillMonster KillMonsterEvent;
	FOnKillAssist KillAssistEvent;//점수판용
	FOnKillPlayer KillPlayerEvent;//점수판 및 킬로그용
	FOnKillCountPlus KillCountPlusEvent;

	FOnKillAlarm KillAlarmEvent;//당신이 ~를 처치했습니다 -> 알람용
	FOnAssistAlarm AssistAlarmEvent;//당신이 ~ 처치에 도움을 주었습니다 -> 알람용
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* PerspectiveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* ZoomHoldAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* ZoomQuickAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* ShotAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* InventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* ScoreAction;

	UPROPERTY(EditAnywhere)
	EEntityType EntityType;
	
	UPROPERTY(Replicated)
	float AimPitch;
	UPROPERTY(BlueprintReadOnly)
	FTransform LeftHandIK;

public:
	//생성자
	APlayerCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE UPlayerCombatSystem* GetCombatComponent() const { return CombatSystem; }
	FORCEINLINE UPlayerStatSystem* GetStatComponent() const { return StatSystem; }
	FORCEINLINE bool GetAiming() const { return bIsAiming; }
	FORCEINLINE bool GetReloading() const { return bIsReloading; }
	FORCEINLINE UInventory* GetInventory() const{ return Inventory; }
	FORCEINLINE bool IsDead() const { return bIsDead; }
	virtual bool GetIsDead() override;
	
	
	void RequestAddItem(UItemData* ItemData, int Amount = 1);
	bool HasWeapon() const;

	virtual EEntityType GetEntityType() override;
	virtual void TakeDamage(FDamageContext& Context) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	// virtual void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartZoom();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartZoomReverse();
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UCameraComponent* FP_Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UCardManager* CardManager;
	UPROPERTY()
	AMyPlayerController* MyController;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	UPlayerWeapon* PlayerWeapon;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComp;
	UPROPERTY(VisibleAnywhere)
	UInventory* Inventory;
	UPROPERTY(EditAnywhere)
	UPlayerCombatSystem* CombatSystem;
	UPROPERTY(EditAnywhere)
	UPlayerStatSystem* StatSystem;

	UPROPERTY(Replicated, ReplicatedUsing=OnRep_bIsDead)
	bool bIsDead;
	bool bIsThirdPerspective;
	
	UPROPERTY(Replicated)
	bool bIsAiming;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_bIsReloading) 
	bool bIsReloading; //애니메이션 모션에 동기화
	bool bCanSprint;
	bool bIsSprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float WalkSpeed = 250.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float SprintSpeed = 500.0f;

	FGenericTeamId TeamId;
	
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void DoMove(float Right, float Forward);
	void StartJump(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprint();
	void ActionCrouch();
	void ZoomHold();
	bool CanZoom() const;
	void ZoomHoldRelease();
	void ZoomQuick();
	void Shot();
	void ShotRelease();
	void ChangePerspective();
	void Reload();
	bool CanReload();
	void PlayReloadMontage();
	void CheckPlayerMovement(float Delta);//?
	void ToggleInventory();
	void OpenScoreBoard();
	void CloseScoreBoard();
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	
	UFUNCTION(Server, Unreliable)
	void Server_ChangeAimPitch(float Pitch);

	UFUNCTION(Server, Unreliable)
	void Server_ChangeZoomState(bool IsZoom);

	UFUNCTION(Server, Reliable)
	void Server_ChangeSpeed(float Speed);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_ChangeSpeed(float Speed);

	UFUNCTION(Server, Reliable)
	void Server_TryReload();
	UFUNCTION()
	void OnRep_bIsReloading();
	UFUNCTION()
	void OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION(Server, Reliable)
	void Server_AddItem(UItemData* ItemData, int Amount = 1);
	void AddItem(UItemInstance* Item, int Amount = 1);

	UFUNCTION()
	void OnRep_bIsDead();
	void Dead();
	void Revive();
	
	void SetCharacterOptionAliveState();

	
	void GetLeftHandIKTransform();

	UFUNCTION(Client, Reliable)
	void Client_BroadcastKillAlarm(AActor* Victim);
	UFUNCTION(Client, Reliable)
	void Client_BroadcastAssistAlarm(AActor* DeadPlayer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void MakeComponents();
	void BindKey(UEnhancedInputComponent* EIC);
	virtual void Tick(float DeltaSeconds) override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void PostInitializeComponents() override;
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	void SetCharacterOptionDeadState();
	virtual void ApplyDamage(IDamageable* Target, FVector HitLocation, FName BoneName, bool bIsRealBullet) const override;
	void InitPlayerState();
	virtual void KillMonster() override;
	virtual void KillOtherPlayer(AActor* DeadPlayer, bool bIsCriticalKill) override;
	virtual void GetAssist(AActor* DeadPlayer) override;
};
