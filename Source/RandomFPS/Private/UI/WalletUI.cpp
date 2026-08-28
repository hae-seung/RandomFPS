// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WalletUI.h"

#include "Components/TextBlock.h"
#include "GameScene/Player/Components/PlayerWalletSystem.h"


void UWalletUI::Init(UPlayerWalletSystem* WalletSystem)
{
	WalletSystem->OnMoneyChanged.AddUObject(this, &UWalletUI::ChangeGoldText);
	ChangeGoldText(WalletSystem->GetMoney());
}


void UWalletUI::ChangeGoldText(int NewGoldAMount)
{
	GoldText->SetText(FText::Format(
		FText::FromString(TEXT("{0}원")),
		NewGoldAMount));
}
