#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(OnNickNameChangedEvent)
DECLARE_MULTICAST_DELEGATE(OnReadyStateChangedEvent)

UCLASS()
class RANDOMFPS_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMyPlayerState();

	OnNickNameChangedEvent NickNameChangedEvent;
	OnReadyStateChangedEvent ReadyStateChangedEvent;

	FORCEINLINE bool CheckIsRoomMaster() const { return bIsRoomMaster; }
	FORCEINLINE FString GetNickName() const { return PlayerNickName; }
	FORCEINLINE bool GetIsReady() const { return bIsReady; }

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

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	UFUNCTION()
	void OnRep_SetNickName();

	UFUNCTION(Server, Reliable)
	void Server_SetMyNickName(const FString& NewNickName);

	UFUNCTION()
	void OnRep_ReadyState();
};
