// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbility/MyGARoll.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "AbilitySystemComponent.h"

UMyGARoll::UMyGARoll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UMyGARoll::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

    return bResult;
}

void UMyGARoll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AMyPlayer* MyPlayer = CastChecked<AMyPlayer>(ActorInfo->AvatarActor.Get());
    if (!MyPlayer)
    {
        return;
    }

    UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Roll"), MyPlayer->GetRollActionMontage(), 1.f, NAME_None, true, 1.f, 0.f);
    Task->OnCompleted.AddDynamic(this, &UMyGARoll::OnCompleteCallback);
    Task->ReadyForActivation();

    /*UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    static const FGameplayTag RollTag = FGameplayTag::RequestGameplayTag(TEXT("Character.State.Roll"));

    if (ASC)
    {
        ASC->AddLooseGameplayTag(RollTag);
    }*/
}

void UMyGARoll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    /*UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    static const FGameplayTag RollTag = FGameplayTag::RequestGameplayTag(TEXT("Character.State.Roll"));

    if (ASC)
    {
        ASC->RemoveLooseGameplayTag(RollTag);
    }*/

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMyGARoll::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMyGARoll::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    SetCanBeCanceled(true);
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = true;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMyGARoll::OnCompleteCallback()
{
    SetCanBeCanceled(true);
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = false;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}