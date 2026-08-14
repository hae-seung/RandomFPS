// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ReinforceShopUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameScene/InteractableObject/InteractableUIObject/ReinforceShop.h"
#include "GameScene/Player/PlayerCharacter.h"
#include "GameScene/Player/Components/PlayerWalletSystem.h"
#include "GameScene/Player/Components/PlayerWeapon.h"
#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Weapon/GunAbility/GunAbilityModifier.h"
#include "UI/GunAbilityDescriptionUI.h"
#include "UI/ReinforceStarUI.h"

class UGunAbilityDescriptionUI;

void UReinforceShopUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReinforceTab->SetVisibility(ESlateVisibility::Collapsed);
	NoGunWarningTab->SetVisibility(ESlateVisibility::Collapsed);
	
	ReinforceBtn->OnReleased.AddDynamic(this, &UReinforceShopUI::RequestReinforce);
	ExitBtn->OnReleased.AddDynamic(this, &UReinforceShopUI::CloseUI);
	CancelBtn->OnReleased.AddDynamic(this, &UReinforceShopUI::CloseUI);
	
	
	CreateInitStars(PStars, PStarBox);
	CreateInitStars(NStars, NStarBox);
}

void UReinforceShopUI::FirstOpenInit(AReinforceShop* RS, APlayerCharacter* InAPC)
{
	APC = InAPC;
	Client_RS = RS;
}

void UReinforceShopUI::Open()
{
	Super::Open();
	
	Client_Gun = APC->GetWeaponSystem()->GetCurrentEquipGun();
	
	bIsSyncComplete = true;
	
	UserMoneyText->SetText(FText::AsNumber(APC->GetWalletSystem()->GetMoney()));
	if(!APC->GetWeaponSystem()->bHasWeapon)
	{
		NoGunWarningTab->SetVisibility(ESlateVisibility::Visible);
		ReinforceTab->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	NoGunWarningTab->SetVisibility(ESlateVisibility::Collapsed);
	ReinforceTab->SetVisibility(ESlateVisibility::Visible);
	
	
	const int CurLevel = Client_Gun->GetGunLevel();
	if(Client_Gun->IsAwake())
	{
		PGunInfo->SetVisibility(ESlateVisibility::Collapsed);
		NextLevelInfoBox->SetVisibility(ESlateVisibility::Collapsed);
		SuccessProbText->SetVisibility(ESlateVisibility::Collapsed);
		ReinforceBtn->SetVisibility(ESlateVisibility::Collapsed);

		SetGunInfoNext(CurLevel + 1);
		SetGunAwakeStateUI(CurLevel);
		return;
	}

	//일반적인 강화 상태창
	SetGunReinforceStateUI(CurLevel);
}


void UReinforceShopUI::CloseUI()
{
	Client_RS->CloseUI();
}

void UReinforceShopUI::CreateInitStars(TArray<UReinforceStarUI*>& Stars, UHorizontalBox* Box)
{
	Box->ClearChildren();
	
	for(int i = 0; i < 5; i++)
	{
		UReinforceStarUI* NewStarUI = CreateWidget<UReinforceStarUI>(this, WB_ReinforceStarUI);
		NewStarUI->Init();
		NewStarUI->SetStarSize(64,64);
		Stars.Add(NewStarUI);
		Box->AddChild(NewStarUI);
	}
}

void UReinforceShopUI::SetGunReinforceStateUI(int CurLevel)
{
	PGunInfo->SetVisibility(ESlateVisibility::Visible);
	NextLevelInfoBox->SetVisibility(ESlateVisibility::Visible);
	SuccessProbText->SetVisibility(ESlateVisibility::Visible);
	ReinforceBtn->SetVisibility(ESlateVisibility::Visible);
	
	SetGunInfoPrev(CurLevel);
	SetGunInfoNext(CurLevel + 1);
	
	SetGunAbilityTexts(CurLevel, PGunAbilityTexts, PGunAbilityBox);
	
	if(CurLevel < 5)
	{
		//next는 다음레벨
		SetGunAbilityTexts(CurLevel + 1, NGunAbilityTexts, NGunAbilityBox);
	}
	else
	{
		//next는 각성
		SetGunAwakeStateUI(CurLevel);
	}
	
	const FGunAbilityWrapper* Wrapper = Client_Gun->GetAbilityWrapper(CurLevel);
	if(!Wrapper)
		return;

	UpgradeNeedMoneyText->SetText(FText::Format(
		FText::FromString(TEXT("{0}원")),
		Wrapper->NextUpgradeNeedMoney));

	SuccessProbText->SetText(FText::Format(
		FText::FromString(TEXT("성공 : {0}%")),
		Wrapper->NextUpgradeProb));
	
	ReinforceBtn->SetIsEnabled(APC->GetWalletSystem()->GetMoney() >= Wrapper->NextUpgradeNeedMoney);
}

void UReinforceShopUI::SetGunAbilityTexts(
	int CurLevel,
	TArray<UGunAbilityDescriptionUI*>& GunAbilityTexts,
	UVerticalBox* AbilityBox)
{
	const FGunAbilityWrapper* Wrapper = Client_Gun->GetAbilityWrapper(CurLevel);
	if(!Wrapper)
		return;
	
	const int32 AbilityCount = Wrapper->GunAbilityModifiers.Num();

	// 부족하면 풀 확장
	while (GunAbilityTexts.Num() < AbilityCount)
	{
		UGunAbilityDescriptionUI* NewDescription =
			CreateWidget<UGunAbilityDescriptionUI>(this, WB_DescriptionUI);

		NewDescription->SetTextSize(DescriptionFontSize);
		NewDescription->SetPadding(FMargin(0.f,0.f,0.f,8.f));
		NewDescription->SetVisibility(ESlateVisibility::Collapsed);
		GunAbilityTexts.Add(NewDescription);
	}

	// 기존 VerticalBox 비우기
	AbilityBox->ClearChildren();

	// 필요한 개수만 활성화
	for (int32 i = 0; i < GunAbilityTexts.Num(); i++)
	{
		if (i < AbilityCount)
		{
			GunAbilityTexts[i]->SetText(
				Wrapper->GunAbilityModifiers[i]->GetAbilityDescription()
			);

			GunAbilityTexts[i]->SetVisibility(ESlateVisibility::Visible);
			AbilityBox->AddChild(GunAbilityTexts[i]);
		}
		else
		{
			GunAbilityTexts[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UReinforceShopUI::SetGunAwakeStateUI(int CurLevel)
{
	//next info에 각성 총 상태를 쓸거임
	SetGunAbilityTexts(
		CurLevel,NGunAbilityTexts, NGunAbilityBox);
	AddGunAwakeAbilityText();
}

void UReinforceShopUI::AddGunAwakeAbilityText()
{
	const TArray<TObjectPtr<UGunAbilityModifier>>* AwakeAbility = Client_Gun->GetAwakeAbility();
	if(!AwakeAbility)
		return;

	const int32 AwakeAbilityCount = AwakeAbility->Num();

	while(NGunAbilityTexts.Num() < NGunAbilityBox->GetChildrenCount() + AwakeAbilityCount)
	{
		UGunAbilityDescriptionUI* NewDescription = CreateWidget<UGunAbilityDescriptionUI>(this, WB_DescriptionUI);
		NewDescription->SetTextSize(DescriptionFontSize);
		NewDescription->SetVisibility(ESlateVisibility::Collapsed);
		NewDescription->SetPadding(FMargin(0.f,0.f,0.f,8.f));
		NGunAbilityTexts.Add(NewDescription);
	}

	int AbilityIndex = 0;
	const int BoxCount = NGunAbilityBox->GetChildrenCount();
	for(int i = BoxCount; i < BoxCount + AwakeAbilityCount; i++)
	{
		NGunAbilityTexts[i]->SetText((*AwakeAbility)[AbilityIndex]->GetAbilityDescription());
		NGunAbilityTexts[i]->SetVisibility(ESlateVisibility::Visible);
		NGunAbilityBox->AddChild(NGunAbilityTexts[i]);
		AbilityIndex++;
	}
}

void UReinforceShopUI::SetGunInfoPrev(int CurLevel)
{
	PGunInfo->SetVisibility(ESlateVisibility::Visible);
	PGunNameText->SetText(Client_Gun->GunItemData->GetItemName());
	PGunImage->SetBrushFromTexture(Client_Gun->GunItemData->GetItemIcon());
	SetStar(CurLevel, PStars);
}

void UReinforceShopUI::SetGunInfoNext(int NextLevel)
{
	NGunInfo->SetVisibility(ESlateVisibility::Visible);
	NGunNameText->SetText(Client_Gun->GunItemData->GetItemName());
	NGunImage->SetBrushFromTexture(Client_Gun->GunItemData->GetItemIcon());

	if(NextLevel > 5)
	{
		for(int i = 0; i < 5; i++)
		{
			NStars[i]->AwakeStar();
		}
	}
	else
	{
		SetStar(NextLevel, NStars);
	}
}

void UReinforceShopUI::SetStar(int Level, TArray<UReinforceStarUI*>& StarBox)
{
	for(int i = 0; i < 5; i++)
	{
		if(i < Level)
		{
			StarBox[i]->SetReinforce();
		}
		else
		{
			StarBox[i]->SetEmpty();
		}
	}
}

void UReinforceShopUI::RequestReinforce()
{
	if(!bIsSyncComplete)
		return;
	
	SetSync(false);
	Client_RS->RequestReinforce();
}

void UReinforceShopUI::SetSync(bool bState)
{
	bIsSyncComplete = bState;
	ReinforceBtn->SetIsEnabled(bState);
}


void UReinforceShopUI::FailToReinforce()
{
	SetSync(true);
}

void UReinforceShopUI::FailToReinforce(int RemainMoney)
{
	SetSync(true);
	
	UserMoneyText->SetText(FText::Format(
		FText::FromString(TEXT("{0}원")), RemainMoney));
}


void UReinforceShopUI::SuccessAwakeGun(int GunLevel, int RemainMoney)
{
	SetSync(true);

	PGunInfo->SetVisibility(ESlateVisibility::Collapsed);
	NextLevelInfoBox->SetVisibility(ESlateVisibility::Collapsed);
	SuccessProbText->SetVisibility(ESlateVisibility::Collapsed);
	ReinforceBtn->SetVisibility(ESlateVisibility::Collapsed);

	UserMoneyText->SetText(FText::Format(
		FText::FromString(TEXT("{0}원")), RemainMoney));
	
	SetGunInfoNext(GunLevel + 1);
	SetGunAwakeStateUI(GunLevel);
}

void UReinforceShopUI::SuccessReinforceGun(int GunLevel, int RemainMoney)
{
	SetSync(true);
	
	UserMoneyText->SetText(FText::Format(
			FText::FromString(TEXT("{0}원")), RemainMoney));
	
	SetGunReinforceStateUI(GunLevel);
}
