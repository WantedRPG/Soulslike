// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/MyUserWidget.h"
#include "GameplayEffectTypes.h"
#include "MyHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyHpBarUserWidget : public UMyUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void NativeConstruct() override;

	void UpdateHpBar();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvinsibleColor = FLinearColor::Blue;
};
