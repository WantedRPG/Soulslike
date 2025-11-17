// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbility/MyGATakeHit.h"
#include "Character/Player/MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UMyGATakeHit::UMyGATakeHit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyGATakeHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMyPlayer* MyPlayer = CastChecked<AMyPlayer>(ActorInfo->AvatarActor.Get());
	UCharacterMovementComponent* MyPlayerMovement = MyPlayer->GetCharacterMovement();

	if (MyPlayer->IsKnockBack() || MyPlayer->IsDead())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	MyPlayer->SetPlayerMode(EPlayerState::KnockBack);

	/*if (MyPlayerMovement->IsMovingOnGround())
	{
		MyPlayerMovement->SetMovementMode(EMovementMode::MOVE_None);
	}*/

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("PlayAttack"), MyPlayer->GetSTakeHitMontage(), 1.0f, MontageSection
	);

	PlayAttackTask->OnCompleted.AddDynamic(this, &UMyGATakeHit::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UMyGATakeHit::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void UMyGATakeHit::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UMyGATakeHit::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMyGATakeHit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AMyPlayer* MyPlayer = CastChecked<AMyPlayer>(ActorInfo->AvatarActor.Get());
	if (!MyPlayer->IsDead())
	{
		//MyPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		if (MyPlayer->IsKnockBack())
		{
			MyPlayer->SetPlayerMode(EPlayerState::Peace);
		}
	}
}

void UMyGATakeHit::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMyGATakeHit::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}