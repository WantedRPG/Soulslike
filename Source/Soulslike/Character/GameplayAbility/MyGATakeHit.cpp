// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbility/MyGATakeHit.h"
#include "Character/Player/MyPlayer.h"
#include "GameFramework/Character.h"
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

	if (MyPlayer->IsDead())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	int32 KnockBackLevel = 1;
	if (TriggerEventData->EventMagnitude)
	{
		KnockBackLevel = (FMath::Abs(TriggerEventData->EventMagnitude) <= 20.f) ? 1 : 2;
	}

	FVector Dir = (MyPlayer->GetActorLocation() - TriggerEventData->Instigator->GetActorLocation());
	Dir.Z = 0.f;
	Dir = Dir.GetSafeNormal();
	Dir *= TriggerEventData->EventMagnitude;
	MyPlayer->LaunchCharacter(Dir, true, false);

#pragma region SectionToPlay
	// 몬스터가 플레이어를 향한 방향
	FVector Monster = (TriggerEventData->Instigator->GetActorLocation() - MyPlayer->GetActorLocation());
	Monster.Z = 0.f;
	Monster = Monster.GetSafeNormal();

	// 플레이어가 보고 있는 방향
	FVector PlayerFwd = MyPlayer->GetActorForwardVector();
	PlayerFwd.Z = 0.f;
	PlayerFwd = PlayerFwd.GetSafeNormal();

	// 내적
	float Dot = FVector::DotProduct(PlayerFwd, Monster);

	FName SectionToPlay = MontageSection_Bwd_1;
	if (Dot < 0.f)
	{
		SectionToPlay = (KnockBackLevel <= 1) ? MontageSection_Fwd_1 : MontageSection_Fwd_2;
	}
	else
	{
		SectionToPlay = (KnockBackLevel <= 1) ? MontageSection_Bwd_1 : MontageSection_Bwd_2;
	}
#pragma endregion

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = 
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), MyPlayer->GetSTakeHitMontage(), 1.0f, SectionToPlay);

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
}

void UMyGATakeHit::OnCompleteCallback()
{
	SetCanBeCanceled(true);
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMyGATakeHit::OnInterruptedCallback()
{
	SetCanBeCanceled(true);
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}