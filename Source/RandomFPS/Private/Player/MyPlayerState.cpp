#include "GameScene/Player/MyPlayerState.h"

#include "Lobby/LobbyGameState.h"
#include "Net/UnrealNetwork.h"



AMyPlayerState::AMyPlayerState()
{
	bReplicates = true;
	
	PlayerNickName = TEXT("Player");
	bIsReady = false;
	bIsRoomMaster = false;
	Life = 100;
}


void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, PlayerNickName);
	DOREPLIFETIME(AMyPlayerState, bIsRoomMaster);
	DOREPLIFETIME(AMyPlayerState, bIsReady);
	
	DOREPLIFETIME(AMyPlayerState, KDA);
	DOREPLIFETIME(AMyPlayerState, MonsterKill);
	DOREPLIFETIME(AMyPlayerState, Life);
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

float AMyPlayerState::GetKDARating() const
{
	float Rating = KDA.Kill + KDA.Assist;
	if(KDA.Death == 0)
		return Rating;

	return Rating / KDA.Death;
}

void AMyPlayerState::DeadPlayer()
{
	KDA.Death += 1;
	KDAChanged.Broadcast(KDA);

	//게임스테이트로부터 현재 차감될 Life를 가져온 뒤 Life 차감.
	//Life -= ..
	//LifeChanged.Broadcast
}

void AMyPlayerState::KillOtherPlayer()
{
	KDA.Kill += 1;
	KDAChanged.Broadcast(KDA);
}

void AMyPlayerState::Assist(AActor* Victim)
{
	//여기선 매개변수 필요 X
	//Score판에 나타나는 숫자만 +1
	KDA.Assist += 1;
	KDAChanged.Broadcast(KDA);
}

void AMyPlayerState::KillMonster()
{
	MonsterKill++;
	MonsterKillChanged.Broadcast(MonsterKill);
}

void AMyPlayerState::OnRep_ReadyState()
{
	ReadyStateChangedEvent.Broadcast();
}

void AMyPlayerState::OnRep_KDA()
{
	KDAChanged.Broadcast(KDA);
}

void AMyPlayerState::OnRep_MonsterKill()
{
	MonsterKillChanged.Broadcast(MonsterKill);
}

void AMyPlayerState::OnRep_Life()
{
	LifeChanged.Broadcast(Life);
}

