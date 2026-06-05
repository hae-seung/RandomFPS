// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MyGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemUtils.h"

//엔진에서 미리 정의해둔 이름으로 사용
const FName UMyGameInstance::SESSION_NAME = NAME_GameSession;

void UMyGameInstance::Init()
{
	Super::Init();

	//싱글톤 객체를 포인터로 Get
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if(!Subsystem) return;
	
	SessionInterface = Subsystem->GetSessionInterface();

	
	
	AddDelegateHandle();
}

void UMyGameInstance::Shutdown()
{
	RemoveDelegateHandle();
	
	Super::Shutdown();
}


////////////////////////////////////////////////////////////////////////
///
///								CreateSession
///
////////////////////////////////////////////////////////////////////
void UMyGameInstance::TryCreateSession(FString RoomName, int32 MaxPlayerCnt, bool bLanMode)
{
	if(!SessionInterface.IsValid()) return;
	
	PendingRoomName = RoomName;
	PendingMaxPlayerCnt = MaxPlayerCnt;
	bPendingLanMode = false;

	// 현재 프로세스에 세션이 동일한 이름으로 존재하는지 확인
	// => 세션은 프로세스당 1개만 만들도록 보장(스팀이던 LAN이던)
	 if(SessionInterface->GetNamedSession(SESSION_NAME))
	 {
	 	SessionInterface->DestroySession(SESSION_NAME);
	 	return;
	 }
	
	
	CreateSessionInternal();
}



void UMyGameInstance::CreateSessionInternal()
{
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = PendingMaxPlayerCnt;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	
	// 생성된 세션의 방 이름 설정
	SessionSettings.Set(
		FName("ROOM_NAME"), // query용 Key 이름
		PendingRoomName,
		EOnlineDataAdvertisementType::ViaOnlineService
	);
	
	
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}



void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
	if(!WasSuccessful)
	{
		OnCreateSessionFail.Execute();
		return;
	}

	if (Maps[EWorldName::Lobby].IsValid() || Maps[EWorldName::Lobby].LoadSynchronous())
	{
		FString MapPath = Maps[EWorldName::Lobby].GetLongPackageName();

		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel(MapPath + "?listen");
		}
	}
	
}


void UMyGameInstance::OnDestroySessionComplete(FName SessionName, bool WasSuccessful)
{
	//empty
}



////////////////////////////////////////////////////////////////////////
///
///								FindSession
///
////////////////////////////////////////////////////////////////////
void UMyGameInstance::FindSession(bool bSearchLan)
{
	if(!SessionInterface.IsValid())
		return;
	
	
	SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(
			SEARCH_LOBBIES,
			true,
			EOnlineComparisonOp::Equals
		);
	
	
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}


void UMyGameInstance::OnFindSessionComplete(bool WasSuccessful)
{
	if(!WasSuccessful)
		GetEngine()->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("검색실패"));
	if(SessionSearch->SearchResults.Num() <= 0)
	{
		GetEngine()->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("방갯수 0개"));
		return;
	}
	
	OnFindSteamSessionComplete.Execute(SessionSearch->SearchResults);
}




////////////////////////////////////////////////////////////////////////
///
///								JoinSession
///
////////////////////////////////////////////////////////////////////
void UMyGameInstance::TryJoinSession(const FOnlineSessionSearchResult& Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Join Start"));
	SessionInterface->JoinSession(0, SESSION_NAME, Result);
}


void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result != EOnJoinSessionCompleteResult::Success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Join Fail"));
		OnJoinSessionFail.Execute();
		return;
	}

	FString ConnectString;
	if(SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if(PC)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Join Success"));
			OnJoinSessionSuccess.Execute();
			
			PC->ClientTravel(ConnectString, TRAVEL_Absolute);
		}
	}
}






////////////////////////////////////////////////////////////////////////
///
///								DelegateHandle
///
////////////////////////////////////////////////////////////////////
void UMyGameInstance::AddDelegateHandle()
{
	if(SessionInterface.IsValid())
	{
		//싱글 델리게이트를 만들어서 내부의 멀티캐스트 델리게이트에 할당하는 방법임.
		CreateSessionCompleteHandle =
			SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
				FOnCreateSessionCompleteDelegate::CreateUObject(
					this, &UMyGameInstance::OnCreateSessionComplete));

		DestroySessionCompleteHandle =
			SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
				FOnDestroySessionCompleteDelegate::CreateUObject(
					this, &UMyGameInstance::OnDestroySessionComplete));
		
		FindSessionCompleteHandle =
			SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
				FOnFindSessionsCompleteDelegate::CreateUObject(
					this, &UMyGameInstance::OnFindSessionComplete));

		JoinSessionCompleteHandle =
			SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
				FOnJoinSessionCompleteDelegate::CreateUObject(
					this, &UMyGameInstance::OnJoinSessionComplete));
	}
}

void UMyGameInstance::RemoveDelegateHandle()
{
	if(SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteHandle);
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteHandle);
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteHandle);
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteHandle);
	}
}


FString UMyGameInstance::GetWorldURL(EWorldName WorldName)
{
	FString MapURL;
	if(Maps[WorldName].IsValid() || Maps[WorldName].LoadSynchronous())
	{
		MapURL = Maps[WorldName].GetLongPackageName();
		return MapURL;
	}

	return MapURL;
}
