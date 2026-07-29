// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GunAbilityDescriptionUI.h"

#include "Components/TextBlock.h"

void UGunAbilityDescriptionUI::SetText(FText Description)
{
	DescriptionText->SetText(Description);
}
