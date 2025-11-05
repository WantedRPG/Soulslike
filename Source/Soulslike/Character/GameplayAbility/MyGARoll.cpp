// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbility/MyGARoll.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "GameFramework/CharacterMovementComponent.h"

UMyGARoll::UMyGARoll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UMyGARoll::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
    if (!bResult || (RollMontage == nullptr)) 
    {
        return false;
    }

	return true;
}

void UMyGARoll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character) 
    {
        return;
    }

    // 기존 회전값 저장
    SavedActorRotation = Character->GetActorRotation();
    // 애니메이션이 보간
    Character->SetActorRotation(FRotator(0, SavedActorRotation.Yaw + 30, 0));

    UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Roll"), RollMontage, 1.f, NAME_None, true, 1.f, 0.f);

    Task->OnCompleted.AddDynamic(this, &UMyGARoll::OnCompleteCallback);
    Task->ReadyForActivation();
}

void UMyGARoll::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    // 회전값 원상복구
    ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
    Character->SetActorRotation(SavedActorRotation);

    SetCanBeCanceled(true);
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = true;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMyGARoll::OnCompleteCallback()
{
    const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
    ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

    //Character->SetActorLocation(FVector(Character->GetActorLocation().X + 500, Character->GetActorLocation().Y, Character->GetActorLocation().Z));

    Character->SetActorRotation(SavedActorRotation);
    // Character->SetActorRotation(FRotator(0, SavedActorRotation.Yaw + 25, 0));

    SetCanBeCanceled(true);
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = false;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}