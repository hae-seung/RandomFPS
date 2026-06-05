// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "SearchRoomListItem.generated.h"


DECLARE_DELEGATE_OneParam(OnClickedSlotBtn, USearchRoomListItem*)



class UTextBlock;
class UButton;



UCLASS()
class RANDOMFPS_API USearchRoomListItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()



public:
	void Init(OnClickedSlotBtn ClickEvent);
	void SetRoomInfo(const FOnlineSessionSearchResult& Result);
	void SetButtonNormal();
	FORCEINLINE FOnlineSessionSearchResult GetSessionSearchResult(){return SessionSearchResult;}
	

private:

	OnClickedSlotBtn ClickedSlotBtn;
	
	UPROPERTY(meta=(BindWidget))
	UButton* SlotBtn;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RoomNameTxt;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PersonTxt;


	FButtonStyle CachedButtonStyle;
	UPROPERTY(EditAnywhere)
	FLinearColor NormalBtnColor;
	UPROPERTY(EditAnywhere)
	FLinearColor DisableColor;
	UPROPERTY(EditAnywhere)
	FLinearColor SelectedBtnColor;
	
	
	FOnlineSessionSearchResult SessionSearchResult;

private:
	UFUNCTION()
	void OnClickSlotBtn();

	void SetButtonState(bool bIsEnabled, FLinearColor Color);

	virtual void NativeOnInitialized() override;
};
