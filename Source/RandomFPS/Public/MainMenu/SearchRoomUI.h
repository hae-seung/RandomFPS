// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SearchRoomUI.generated.h"

class UWrapBox;
class USearchRoomListItem;
class UButton;
/**
 * 
 */
UCLASS()
class RANDOMFPS_API USearchRoomUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void Open();
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<USearchRoomListItem> BP_Room;
	
	UPROPERTY(meta=(BindWidget))
	UWrapBox* SteamListView;
	UPROPERTY(meta=(BindWidget))
	UButton* CloseBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* JoinBtn;
	
	TArray<USearchRoomListItem*> SteamRooms;

	USearchRoomListItem* CurrentSelectRoom;

private:
	virtual void NativeOnInitialized() override;
	void Init();

	UFUNCTION()
	void OnClickedCloseBtn();
	UFUNCTION()
	void OnClickJoinBtn();
	
	void OnSearchSteamRoomComplete(const TArray<FOnlineSessionSearchResult>& Rooms);
	void OnJoinSessionFail();
	void OnJoinSessionSuccess();

	void SetCurrentSelectRoom(USearchRoomListItem* Room);
};
