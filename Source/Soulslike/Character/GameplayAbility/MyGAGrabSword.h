// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "MyGAGrabSword.generated.h"

class AMyTA_Sword;
class UAbilityTask_WaitGameplayEvent;
class UAnimMontage;

// TODO. 무기를 선택해서 잡을 수 있도록 변경하기

UCLASS()
class SOULSLIKE_API UMyGAGrabSword : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMyGAGrabSword();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

protected:
	UFUNCTION()
	void OnNotifyEventReceived(FGameplayEventData EventData);

	UFUNCTION()
	void OnTraceResultCallback(const struct FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterruptedOrCancelled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab | Montage")
	TObjectPtr<UAnimMontage> GrabMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab | AnimNotify")
	FGameplayTag HitNotifyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab")
	FName HandSocketName = TEXT("hand_rSocket");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grab | Tag")
	FGameplayTag HasWeaponTag;

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask = nullptr;
};
