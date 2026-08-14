// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ReinforceStarUI.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

void UReinforceStarUI::Init()
{
	StarImage->SetBrushFromTexture(EmptyStarImage);
}

void UReinforceStarUI::SetReinforce()
{
	StarImage->SetBrushFromTexture(ReinforceStarImage);
}

void UReinforceStarUI::SetEmpty()
{
	StarImage->SetBrushFromTexture(EmptyStarImage);
}

void UReinforceStarUI::AwakeStar()
{
	StarImage->SetBrushFromTexture(AwakeStarImage);
}

void UReinforceStarUI::SetStarSize(float X, float Y)
{
	StarSize->SetWidthOverride(X);
	StarSize->SetHeightOverride(Y);
}

