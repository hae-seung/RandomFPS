#include "GameScene/Player/MyPlayerState.h"

#include "Lobby/LobbyGameState.h"
#include "Net/UnrealNetwork.h"



AMyPlayerState::AMyPlayerState()
{
	PlayerNickName = TEXT("Player");
	bIsReady = false;
	bIsRoomMaster = false;
}


void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, PlayerNickName);
	DOREPLIFETIME(AMyPlayerState, bIsRoomMaster);
	DOREPLIFETIME(AMyPlayerState, bIsReady);
}

void AMyPlayerState::RequestSetNickName(const FText& NewNickName)
{
	const FString NewName = NewNickName.ToString();

	if (!HasAuthority())
	{
		Server_SetMyNickName(NewName);
	}
	else
	{
		PlayerNickName = NewName;
		NickNameChangedEvent.Broadcast();
	}
}

void AMyPlayerState::Server_SetMyNickName_Implementation(const FString& NewNickName)
{
	PlayerNickName = NewNickName;
	NickNameChangedEvent.Broadcast();
}

void AMyPlayerState::OnRep_SetNickName()
{
	NickNameChangedEvent.Broadcast();
}

void AMyPlayerState::SetRoomMaster()
{
	if (HasAuthority())
	{
		bIsRoomMaster = true;
	}
}

void AMyPlayerState::Server_SetReadyState_Implementation()
{
	bIsReady = !bIsReady;
	ReadyStateChangedEvent.Broadcast();

	ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>();
	if(GS)
	{
		GS->SetReadyState(bIsReady);
	}
}

void AMyPlayerState::OnRep_ReadyState()
{
	ReadyStateChangedEvent.Broadcast();
}
