// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/MyWidgetComponent.h"
#include "Character/UI/MyUserWidget.h"

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMyUserWidget* GASUserWidget = Cast<UMyUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
