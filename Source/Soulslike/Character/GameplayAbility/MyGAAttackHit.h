// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "MyGAAttackHit.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyGAAttackHit : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMyGAAttackHit();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;
	
	float CurrentLevel;

protected:
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* HitNiagara;

	UPROPERTY(EditAnywhere, Category = "VFX")
	FName HitNiagaraSocketName = TEXT("hand_rSocket");
};
