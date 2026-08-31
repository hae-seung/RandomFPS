// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CheatButton.h"

void UCheatButton::Init()
{
	OnClicked.AddDynamic(this, &UCheatButton::CheatBtnClicked);
}

void UCheatButton::CheatBtnClicked()
{
	OnCheatBtnClicked.Execute(ItemData);
}
