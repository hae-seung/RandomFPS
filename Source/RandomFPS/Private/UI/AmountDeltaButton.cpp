// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AmountDeltaButton.h"


void UAmountDeltaButton::InitFirstOpen()
{
	OnReleased.AddDynamic(this, &UAmountDeltaButton::HandleRelease);
}


void UAmountDeltaButton::HandleRelease()
{
	OnCustomButtonReleased.Broadcast(AmountDelta);
}
