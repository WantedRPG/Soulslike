// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/MyUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UMyUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UMyUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
