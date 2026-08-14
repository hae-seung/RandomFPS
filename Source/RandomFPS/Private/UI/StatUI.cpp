// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatUI.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GameScene/Player/Components/PlayerStatSystem.h"
#include "GameScene/Player/Components/PlayerWeapon.h"
#include "GameScene/Player/ItemData/GunItemData.h"
#include "GameScene/Player/ItemInstance/GunItem.h"
#include "GameScene/Weapon/GunAbility/GunAbilityModifier.h"
#include "Public/UI/GunAbilityDescriptionUI.h"
#include "Public/UI/ReinforceStarUI.h"

void UStatUI::Init(UPlayerStatSystem* PlayerStatSystem, UPlayerWeapon* PlayerWeapon)
{
	StatSystem = PlayerStatSystem;
	
	ZeroDigit.MaximumFractionalDigits = 0;
	OneDigit.MinimumFractionalDigits = 1;
	OneDigit.MaximumFractionalDigits = 1;
	
	StatSystem->OnPlayerHealthStatChanged.AddUObject(this, &UStatUI::UpdateHealthStat);
	StatSystem->OnPlayerCombatStatChanged.AddUObject(this, &UStatUI::UpdateCombatStat);
	StatSystem->OnPlayerUtilityStatChanged.AddUObject(this, &UStatUI::UpdateUtilityStat);
	StatSystem->InitDelegates();

	GunImage->SetVisibility(ESlateVisibility::Hidden);
	StarBox->ClearChildren();
	
	//MaxLevel이 5라고 가정하고 5개 초기화.
	for(int i = 0; i < 5; i++)
	{
		UReinforceStarUI* NewStarUI = CreateWidget<UReinforceStarUI>(this, BP_StartUI);
		NewStarUI->Init();
		Stars.Add(NewStarUI);

		StarBox->AddChildToHorizontalBox(NewStarUI);
	}
	StarBox->SetVisibility(ESlateVisibility::Hidden);
	
	PlayerWeapon->OnChangeGunInstance.AddUObject(this, &UStatUI::ChangeGunInstance);
}

void UStatUI::UpdateHealthStat(const FPlayerHealthStat& HealthStat)
{
	MaxHpText->SetText(FText::AsNumber(HealthStat.MaxHp, &ZeroDigit));
	HpText->SetText(FText::AsNumber(HealthStat.Hp, &ZeroDigit));
}

void UStatUI::UpdateCombatStat(const FPlayerCombatStat& CombatStat)
{
	AttackDamageText->SetText(FText::AsNumber(CombatStat.AttackDamage, &ZeroDigit));
	
	HeadShotDamageText->SetText(
		FText::Format(
		FText::FromString(TEXT("X {0}")),
		FText::AsNumber(CombatStat.CriticalDamageBonus, &OneDigit)
	));
	
	DefenseText->SetText(FText::AsNumber(CombatStat.Defense, &ZeroDigit));
	
	DefenseIgnoreRateText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}%")),
			FText::AsNumber(CombatStat.DismissDefenseRate * 100, &ZeroDigit)
			));
	
	HeadDamageIgnoreText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}%")),
			FText::AsNumber(CombatStat.CriticalDamageDefense * 100, &ZeroDigit)
			));
	
	MonsterFlinchRateText->SetText(
		FText::Format(
			FText::FromString(TEXT("{0}%")),
			FText::AsNumber(CombatStat.MonsterFlinchProbability, &ZeroDigit)
			));
}

void UStatUI::UpdateUtilityStat(const FPlayerUtilityStat& UtilityStat)
{
	UE_LOG(LogTemp, Warning, TEXT("UI WalkSpeed = %f"), UtilityStat.WalkSpeed);
	WalkSpeedText->SetText(FText::AsNumber(UtilityStat.WalkSpeed, &ZeroDigit));
	ReviveTimeText->SetText(
		FText::Format(
		FText::FromString(TEXT("{0}초")),
		FText::AsNumber(UtilityStat.ReviveTime, &ZeroDigit)));
}


void UStatUI::ChangeGunInstance(UGunItem* GunItem)
{
	CurrentGun = GunItem;
	
	CurrentGun->OnGunLevelChanged.AddUObject(this, &UStatUI::UpdateGunAbility);
	CurrentGun->OnGunLevelChanged.AddUObject(this, &UStatUI::UpdateGunStar);
	CurrentGun->OnGunAwake.AddUObject(this, &UStatUI::UpdateAwakeStar);
	CurrentGun->OnGunAwake.AddUObject(this, &UStatUI::UpdateGunAwakeAbility);

	
	UpdateGunAbility(CurrentGun->GetGunLevel());

	//일단 바뀐 총의 상태로 업데이트
	if(CurrentGun->IsAwake())
	{
		UpdateGunAwakeAbility();
		UpdateAwakeStar();
	}
	else
	{
		UpdateGunStar(CurrentGun->GetGunLevel());
	}
	
	GunImage->SetVisibility(ESlateVisibility::Visible);
	GunImage->SetBrushFromTexture(GunItem->ItemData->GetItemIcon());

	StarBox->SetVisibility(ESlateVisibility::Visible);
}

void UStatUI::UpdateGunAbility(int Level)
{
	const FGunAbilityWrapper* Wrapper = CurrentGun->GetAbilityWrapper(Level);
	if(!Wrapper)
		return;
	
	const int32 AbilityCount = Wrapper->GunAbilityModifiers.Num();

	// 부족하면 풀 확장
	while (GunDescriptions.Num() < AbilityCount)
	{
		UGunAbilityDescriptionUI* NewDescription =
			CreateWidget<UGunAbilityDescriptionUI>(this, BP_DescriptionUI);

		if (NewDescription)
		{
			NewDescription->SetVisibility(ESlateVisibility::Collapsed);
			GunDescriptions.Add(NewDescription);
		}
		else
		{
			break;
		}
	}

	// 기존 VerticalBox 비우기
	DescriptionListBox->ClearChildren();

	// 필요한 개수만 활성화
	for (int32 i = 0; i < GunDescriptions.Num(); i++)
	{
		if (i < AbilityCount)
		{
			GunDescriptions[i]->SetText(
				Wrapper->GunAbilityModifiers[i]->GetAbilityDescription()
			);

			GunDescriptions[i]->SetVisibility(ESlateVisibility::Visible);

			UVerticalBoxSlot* BoxSlot =  DescriptionListBox->AddChildToVerticalBox(GunDescriptions[i]);

			//패딩
			if(i > 0)
			{
				BoxSlot->SetPadding(FMargin(0.f,10.f,0.f,0.f));
			}
			else
			{
				BoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
			}
		}
		else
		{
			GunDescriptions[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UStatUI::UpdateGunAwakeAbility()
{
	const TArray<TObjectPtr<UGunAbilityModifier>>* AwakeAbility = CurrentGun->GetAwakeAbility();
	if(!AwakeAbility)
		return;
	
	const int32 AwakeAbilityCount = AwakeAbility->Num();

	for(int i = 0; i < AwakeAbilityCount; i++)
	{
		UGunAbilityDescriptionUI* NewDescriptionUI =
			CreateWidget<UGunAbilityDescriptionUI>(this, BP_DescriptionUI);

		NewDescriptionUI->SetVisibility(ESlateVisibility::Visible);
		NewDescriptionUI->SetText((*AwakeAbility)[i]->GetAbilityDescription());
		UVerticalBoxSlot* BoxSlot = DescriptionListBox->AddChildToVerticalBox(NewDescriptionUI);

		if(i < AwakeAbilityCount - 1)
		{
			BoxSlot->SetPadding(FMargin(0.f,10.f,0.f,0.f));
		}
	}
}


void UStatUI::UpdateGunStar(int Level)
{
	//MaxLevel이 5라고 가정
	for(int i = 0; i < 5; i++)
	{
		if(i < Level)
		{
			Stars[i]->SetReinforce();
		}
		else
		{
			Stars[i]->SetEmpty();
		}
	}
}

void UStatUI::UpdateAwakeStar()
{
	//MaxLevel이 5라고 가정
	for(int i = 0; i < 5; i++)
	{
		Stars[i]->AwakeStar();
	}
}