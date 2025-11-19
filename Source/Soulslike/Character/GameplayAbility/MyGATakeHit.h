// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyGATakeHit.generated.h"

UCLASS()
class SOULSLIKE_API UMyGATakeHit : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMyGATakeHit();

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage | Section", meta = (AllowPrivateAccess = "true"))
	FName MontageSection1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage | Section", meta = (AllowPrivateAccess = "true"))
	FName MontageSection2;
};
