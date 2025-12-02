// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/MyHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/SLAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/Tag/MyGameplayTag.h"

void UMyHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(USLAttributeSet::GetHealthAttribute()).AddUObject(this, &UMyHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(USLAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UMyHpBarUserWidget::OnMaxHealthChanged);
		ASC->RegisterGameplayTagEvent(MyTag_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UMyHpBarUserWidget::OnInvinsibleTagChanged);
		PbHpBar->SetFillColorAndOpacity(HealthColor);

		const USLAttributeSet* CurrentAttributeSet = ASC->GetSet<USLAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}
		}
	}
}

void UMyHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UMyHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UMyHpBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvinsibleColor);
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
}

void UMyHpBarUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TxtHpStat)
	{
		FSlateFontInfo FontInfo = TxtHpStat->GetFont();
		FontInfo.Size = 17;
		TxtHpStat->SetFont(FontInfo);
	}
}

void UMyHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
