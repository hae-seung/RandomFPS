// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScene/Player/AnimInstance/CharacterLinkedAnimLayer.h"
#include "GameScene/Player/CharacterAnimInstance.h"


UCharacterAnimInstance* UCharacterLinkedAnimLayer::GetCharacterAnimInstance() const
{
	return Cast<UCharacterAnimInstance>(GetOwningComponent()->GetAnimInstance());
}

