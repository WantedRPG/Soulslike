// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbility/MyGAComboAttack.h"
#include "Character/Player/MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UMyGAComboAttack::UMyGAComboAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyGAComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMyPlayer* MyPlayer = CastChecked<AMyPlayer>(ActorInfo->AvatarActor.Get());
	UCharacterMovementComponent* MyPlayerMovement = MyPlayer->GetCharacterMovement();

	CurrentComboAttackData = MyPlayer->GetComboActionData();

	if (MyPlayerMovement->IsMovingOnGround())
	{
		MyPlayerMovement->SetMovementMode(EMovementMode::MOVE_None);
	}

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("PlayAttack"), MyPlayer->GetComboActionMontage(), 1.0f, GetNextSection()/*재생할 섹션*/
	);

	PlayAttackTask->OnCompleted.AddDynamic(this, &UMyGAComboAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UMyGAComboAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation(); 

	StartComboTimer();
}

void UMyGAComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	HasNextComboInput = ComboTimerHandle.IsValid();
}

void UMyGAComboAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMyGAComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AMyPlayer* MyPlayer = CastChecked<AMyPlayer>(ActorInfo->AvatarActor.Get());
	MyPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	CurrentComboAttackData = nullptr;
	CurrentComboIx = -1;
	HasNextComboInput = false;
}

void UMyGAComboAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMyGAComboAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UMyGAComboAttack::GetNextSection()
{
	const int32 MaxIx = CurrentComboAttackData->ComboSectionNames.Num();

	// 마지막 섹션이면 다음 섹션으로 전환하지 않음.
	if (!CurrentComboAttackData || (CurrentComboIx >= MaxIx - 1) || (MaxIx <= 0))
	{
		return NAME_None;
	}

	CurrentComboIx = FMath::Clamp(CurrentComboIx + 1, 0, MaxIx - 1);
	FName NextSection = CurrentComboAttackData->ComboSectionNames[CurrentComboIx];

	return NextSection;
}

void UMyGAComboAttack::StartComboTimer()
{
	const int32 MaxIx = CurrentComboAttackData->CheckComboFrameCount.Num();

	// 타이머 비활성화
	if (!CurrentComboAttackData || (CurrentComboIx >= MaxIx - 1))
	{
		return;
	}

	// EffectiveFrameCount 배열에 해당 인덱스가 존재하는지
	if (!CurrentComboAttackData->CheckComboFrameCount.IsValidIndex(CurrentComboIx))
	{
		return;
	}

	const float FrameRate = FMath::Max(1.0f, CurrentComboAttackData->FrameRate);

	// EffectiveFrameCount를 초 단위로 환산
	const float ComboEffectiveTime = CurrentComboAttackData->CheckComboFrameCount[CurrentComboIx] / FrameRate;

	if (ComboEffectiveTime > 0.f)
	{
		// 타이머 활성화
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UMyGAComboAttack::CheckComboInput/*타이머 만료 시 호출*/, ComboEffectiveTime/*타이머 시간*/, false);
	}
}

void UMyGAComboAttack::CheckComboInput()
{
	// 타이머 무효화
	ComboTimerHandle.Invalidate();

	if (!HasNextComboInput)
	{
		return;
	}

	const FName Next = GetNextSection();
	if (Next != NAME_None)
	{
		// 다음 몽타주 재생
		MontageJumpToSection(Next);
		StartComboTimer();
	}

	HasNextComboInput = false;
}
