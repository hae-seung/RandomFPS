// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "OnlineSubsystem.h"					//build.cs의 "OnlineSubsystem" 필요
#include "GameScene/EnumHeader/EnumHeader.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"



DECLARE_DELEGATE(FOnCreateSessionFailEvent);
DECLARE_DELEGATE_OneParam(FOnFindSteamSessionCompleteEvent, const TArray<FOnlineSessionSearchResult>&);
DECLARE_DELEGATE(FOnJoinSessionFailEvent);
DECLARE_DELEGATE(FOnJoinSessionSuccessEvent);


class FOnlineSessionSearch;

UCLASS()
class RANDOMFPS_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//로컬에서 세션객체를 식별하는 이름
	//=>이 프로세스에서 이 세션 객체를 뭐라고 부를 것인가.
	static const FName SESSION_NAME;
	
	FOnCreateSessionFailEvent OnCreateSessionFail;
	FOnFindSteamSessionCompleteEvent OnFindSteamSessionComplete;
	FOnJoinSessionFailEvent OnJoinSessionFail;
	FOnJoinSessionSuccessEvent OnJoinSessionSuccess;
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	

	FString GetWorldURL(EWorldName WorldName);
	void FindSession(bool bSearchLan = false);
	void TryCreateSession(FString RoomName, int32 MaxPlayerCnt, bool bLanMode = false);
	void TryJoinSession(const FOnlineSessionSearchResult& Result);
	
private:
	//TSharedPtr로 참조카운트 기반 C++객체임. <포인터처럼 쓸 수 있도록 operator를 다 바꿈 내부에서>
	//엔진 내부 GC대상 아님 => PIE종료해도 계속 살아있음.
	IOnlineSessionPtr SessionInterface; //OnlineSubSystem.h

	//검색을 위한 스마트 포인터
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(EditAnywhere)
	TMap<EWorldName, TSoftObjectPtr<UWorld>> Maps;

	FDelegateHandle CreateSessionCompleteHandle;
	FDelegateHandle DestroySessionCompleteHandle;
	FDelegateHandle FindSessionCompleteHandle;
	FDelegateHandle JoinSessionCompleteHandle;

	FString PendingRoomName;
	int32 PendingMaxPlayerCnt;
	bool bPendingLanMode;
	

	
private:
	void OnCreateSessionComplete(FName SessionName, bool WasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool WasSuccessful);
	void OnFindSessionComplete(bool WasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	void CreateSessionInternal();

	void AddDelegateHandle();
	void RemoveDelegateHandle();
};
