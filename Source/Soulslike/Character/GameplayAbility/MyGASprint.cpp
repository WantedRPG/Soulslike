// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbility/MyGASprint.h"
#include "GameFramework/Character.h"          
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/SLAttributeSet.h"

UMyGASprint::UMyGASprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UMyGASprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (!bResult)
	{
		return false;
	}

	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	const UCharacterMovementComponent* Move = Character->GetCharacterMovement();

	if (!Character || !Move) 
	{
		return false;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
	const USLAttributeSet* SourceAttributeSet = SourceASC->GetSet<USLAttributeSet>();

	if (!SourceASC || !SourceAttributeSet)
	{
		return false;
	}

	// 스태미너가 완전히 채워져야 전력질주 가능 (자동으로 스태미너가 채워질 때까지 대기해야 하므로, 별도의 타이머 불필요)
	if (SourceAttributeSet->GetStamina() <= 50.f)
	{
		return false;
	}

	return true;
}

void UMyGASprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (SourceASC && SprintDrainEffect)
	{
		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(SprintDrainEffect, 1.f, Context);
		if (Spec.IsValid())
		{
			SprintDrainEffectHandle = SourceASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}

	// TODO. AT 연결
}

void UMyGASprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC && SprintDrainEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(SprintDrainEffectHandle);
		SprintDrainEffectHandle.Invalidate();
	}
}

void UMyGASprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC && SprintDrainEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(SprintDrainEffectHandle);
		SprintDrainEffectHandle.Invalidate();
	}
}

void UMyGASprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}


void UMyGASprint::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
