// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/MyUserWidget.h"
#include "GameplayEffectTypes.h"
#include "MyStaminaBarUserWidget.generated.h"

// TODO. 스테미나 max인지 아닌지만 나오게 수정하자... 뭔가 유저 입장에서는 스테미나가 남아 있으면 사용 가능하다고 착각 가능

UCLASS()
class SOULSLIKE_API UMyStaminaBarUserWidget : public UMyUserWidget
{
	GENERATED_BODY()

protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnStaminaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxStaminaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void NativeConstruct() override;

	void UpdateStaminaBar();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbStaminaBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtStaminaStat;

	float CurrentStamina = 0.0f;
	float CurrentMaxStamina = 0.1f;

	FLinearColor StaminaColor = FLinearColor::Blue;
	FLinearColor InvinsibleColor = FLinearColor::Blue;
};
