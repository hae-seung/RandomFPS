#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Struct/CombatStructHeader.h"
#include "MyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(OnNickNameChangedEvent)
DECLARE_MULTICAST_DELEGATE(OnReadyStateChangedEvent)

DECLARE_MULTICAST_DELEGATE_OneParam(OnKDAChanged, const FPlayerKDA&);
DECLARE_MULTICAST_DELEGATE_OneParam(OnMonsterKillChanged, int);
DECLARE_MULTICAST_DELEGATE_OneParam(OnLifeChanged, int);


UCLASS()
class RANDOMFPS_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	OnNickNameChangedEvent NickNameChangedEvent;
	OnReadyStateChangedEvent ReadyStateChangedEvent;

	OnKDAChanged KDAChanged;
	OnMonsterKillChanged MonsterKillChanged;
	OnLifeChanged LifeChanged;
	


public:
	AMyPlayerState();
	
	FORCEINLINE bool CheckIsRoomMaster() const { return bIsRoomMaster; }
	FORCEINLINE FString GetNickName() const { return PlayerNickName; }
	FORCEINLINE bool GetIsReady() const { return bIsReady; }
	FORCEINLINE const FPlayerKDA& GetKDA() const { return KDA; }
	FORCEINLINE int GetMonsterKill() const { return MonsterKill; }
	FORCEINLINE int GetLife() const { return Life; }
	float GetKDARating() const;


	void DeadPlayer();
	void KillOtherPlayer(); //k d a 에서 k++;
	void Assist(AActor* Victim);
	void KillMonster();
	
	void SetRoomMaster();
	void RequestSetNickName(const FText& NewNickName);

	UFUNCTION(Server, Reliable)
	void Server_SetReadyState();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_SetNickName)
	FString PlayerNickName;
	UPROPERTY(Replicated)
	bool bIsRoomMaster = false;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_ReadyState)
	bool bIsReady = false;

	UPROPERTY(Replicated, ReplicatedUsing=OnRep_KDA)
	FPlayerKDA KDA;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_MonsterKill)
	int MonsterKill;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_Life)
	int Life;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	UFUNCTION()
	void OnRep_SetNickName();
	UFUNCTION(Server, Reliable)
	void Server_SetMyNickName(const FString& NewNickName);
	UFUNCTION()
	void OnRep_ReadyState();
	UFUNCTION()
	void OnRep_KDA();
	UFUNCTION()
	void OnRep_MonsterKill();
	UFUNCTION()
	void OnRep_Life();

	
};
