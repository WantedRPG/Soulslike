// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MyPlayerState.h"
#include "AbilitySystemComponent.h"

AMyPlayerState::AMyPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	// AttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
