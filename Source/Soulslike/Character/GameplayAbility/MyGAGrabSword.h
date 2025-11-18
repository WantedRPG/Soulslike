// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "MyGAGrabSword.generated.h"

class AMyTA_Sword;
class UAbilityTask_WaitGameplayEvent;
class UAnimMontage;

// TODO. 
// 1. 무기가 소켓에 장착되도록 수정. TA를 참고해서 태그도 넣어야 함.
// 2. 무기 장착 여부에 따라 근접 공격과 무기 공격으로 나뉠 것

// 3. 무기가 손에 있을 경우는 실행 불가.

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
