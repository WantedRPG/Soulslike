// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyGAComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyGAComboAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMyGAComboAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	FName GetNextSection();

	void StartComboTimer();
	void CheckComboInput();

protected:
	UPROPERTY(EditAnywhere, Category = "Attack | Melee")
	TObjectPtr<class UMyPDAComboAttack> CurrentComboAttackData;

	int32 CurrentComboIx = -1;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;
};
