// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AbilityTask/MyATSprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMyATSprint::UMyATSprint()
{
}

UMyATSprint* UMyATSprint::CreateTask(UGameplayAbility* OwningAbility)
{
	UMyATSprint* NewTask = NewAbilityTask<UMyATSprint>(OwningAbility);
	return NewTask;
}

void UMyATSprint::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->GetCharacterMovement()->MaxWalkSpeed = SprintMovementSpeed;

	SetWaitingOnAvatar();
}

void UMyATSprint::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;

	Super::OnDestroy(AbilityEnded);
}

void UMyATSprint::OnSprintCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
