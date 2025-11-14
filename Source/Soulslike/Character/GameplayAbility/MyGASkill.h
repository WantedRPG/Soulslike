// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyGASkill.generated.h"

// TODO. 부모 스킬 GA를 만들어서, 스킬별로 BPGA만 다르게 할까?
// 뭐 여기서 몽타주만 처리하면 괜찮을 것 같아.
// 
// TODO. 무기가 있을 때만 skill 공격이 가능하도록 수정해야 함. 태그로.
// 
// TODO. 각 GA마다 서로 다른 GE를 받아야 하는데? 이건 또 Player 코드랑 연관됨. ★★★
// 근데 공격/상태 이상 등 계산 같은 게 달라지는 데 그걸 어떻게 처리할지 고민해 보기

UCLASS()
class SOULSLIKE_API UMyGASkill : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMyGASkill();

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
