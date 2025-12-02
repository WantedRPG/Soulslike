// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/MyStaminaBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/SLAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/Tag/MyGameplayTag.h"

void UMyStaminaBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(USLAttributeSet::GetStaminaAttribute()).AddUObject(this, &UMyStaminaBarUserWidget::OnStaminaChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(USLAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &UMyStaminaBarUserWidget::OnMaxStaminaChanged);
		ASC->RegisterGameplayTagEvent(MyTag_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UMyStaminaBarUserWidget::OnInvinsibleTagChanged);
		PbStaminaBar->SetFillColorAndOpacity(StaminaColor);

		const USLAttributeSet* CurrentAttributeSet = ASC->GetSet<USLAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentStamina = CurrentAttributeSet->GetStamina();
			CurrentMaxStamina = CurrentAttributeSet->GetMaxStamina();

			if (CurrentMaxStamina > 0.0f)
			{
				UpdateStaminaBar();
			}
		}
	}
}

void UMyStaminaBarUserWidget::OnStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentStamina = ChangeData.NewValue;
	UpdateStaminaBar();
}

void UMyStaminaBarUserWidget::OnMaxStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxStamina = ChangeData.NewValue;
	UpdateStaminaBar();
}

void UMyStaminaBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbStaminaBar->SetFillColorAndOpacity(InvinsibleColor);
		PbStaminaBar->SetPercent(1.0f);
	}
	else
	{
		PbStaminaBar->SetFillColorAndOpacity(StaminaColor);
		UpdateStaminaBar();
	}
}

void UMyStaminaBarUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TxtStaminaStat)
	{
		FSlateFontInfo FontInfo = TxtStaminaStat->GetFont();
		FontInfo.Size = 15;         
		TxtStaminaStat->SetFont(FontInfo);
	}
}

void UMyStaminaBarUserWidget::UpdateStaminaBar()
{
	if (PbStaminaBar)
	{
		PbStaminaBar->SetPercent(CurrentStamina / CurrentMaxStamina);
	}

	if (TxtStaminaStat)
	{
		TxtStaminaStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentStamina, CurrentMaxStamina)));
	}
}
