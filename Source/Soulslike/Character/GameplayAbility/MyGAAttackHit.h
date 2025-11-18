// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
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
	
	// 공격별 Attack 값 지정
	/*UPROPERTY(EditAnywhere, Category = "GAS | GE")
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> EffectByAttackTag;*/

	float CurrentLevel;

	//FGameplayTag AttackTag;
};
