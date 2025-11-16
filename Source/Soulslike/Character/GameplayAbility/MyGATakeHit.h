// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyGATakeHit.generated.h"

// TODO. 피격 당했을 때 재생할 GA (InputGA 아님.)
// (1.) 넉백은 GA 몽타주 피격 둥울 처리할 예정. 누적치도...

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
	FName MontageSection;
};
