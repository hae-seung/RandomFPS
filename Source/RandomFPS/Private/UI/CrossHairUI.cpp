// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CrossHairUI.h"

#include "Components/Image.h"
#include "GameScene/Player/Components/PlayerCombatSystem.h"

void UCrossHairUI::Init(UPlayerCombatSystem* CombatSystem)
{
	HitHair->SetVisibility(ESlateVisibility::Collapsed);

	CombatSystem->OnHitSuccess.AddUObject(this, &UCrossHairUI::HitSuccess);
}


void UCrossHairUI::MoveCrossHair()
{
	
}

void UCrossHairUI::HitSuccess(bool bIsCritical)
{
	HitHair->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	if(bIsCritical)
	{
		PlayAnimation(CriticalHitEffectAnim);
	}
	else
	{
		PlayAnimation(HitEffectAnim);
	}
}
