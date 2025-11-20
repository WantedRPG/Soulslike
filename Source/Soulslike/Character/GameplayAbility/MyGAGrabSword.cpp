// Fill out your copyright notice in the Description page of Project Settings.
// MyGAGrabSword.cpp

#include "MyGAGrabSword.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/AbilityTask/MyATWeaponHit.h"           
#include "Character/GameplayAbilityTargetActor/MyTA_Sword.h"      

#include "AbilitySystemComponent.h"   
#include "GameplayAbilitySpec.h" 
#include <Components/BoxComponent.h>
#include <Character/Player/MyPlayerController.h>

UMyGAGrabSword::UMyGAGrabSword()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyGAGrabSword::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMyPlayer* MyPlayer = CastChecked<AMyPlayer>(ActorInfo->AvatarActor.Get());
	UCharacterMovementComponent* MyPlayerMovement = MyPlayer->GetCharacterMovement();

	if (MyPlayerMovement->IsMovingOnGround())
	{
		MyPlayerMovement->SetMovementMode(EMovementMode::MOVE_None);
	}

	// 몽타주 AT
	if (GrabMontage)
	{
		if (UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, TEXT("GrabMontage"), GrabMontage, 2.f/*rate*/, TEXT("Grab"), true, 1.f))
		{
			MontageTask->OnCompleted.AddDynamic(this, &UMyGAGrabSword::OnMontageCompleted);
			MontageTask->OnInterrupted.AddDynamic(this, &UMyGAGrabSword::OnMontageInterruptedOrCancelled);
			MontageTask->OnCancelled.AddDynamic(this, &UMyGAGrabSword::OnMontageInterruptedOrCancelled);
			MontageTask->ReadyForActivation();
		}
	}

	// 노티파이가 활성화 → 충돌 AT
	if (HitNotifyTag.IsValid())
	{
		WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, HitNotifyTag, nullptr, /*OnlyTriggerOnce*/false, /*OnlyMatchExact*/true);
		if (WaitEventTask)
		{
			WaitEventTask->EventReceived.AddDynamic(this, &UMyGAGrabSword::OnNotifyEventReceived);
			WaitEventTask->ReadyForActivation();
		}
	}
}

void UMyGAGrabSword::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AMyPlayer* MyPlayer = CastChecked<AMyPlayer>(ActorInfo->AvatarActor.Get());
	MyPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UMyGAGrabSword::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMyGAGrabSword::OnNotifyEventReceived(FGameplayEventData EventData)
{
	TSubclassOf<AMyTA_Sword> TargetActorClass = AMyTA_Sword::StaticClass();

	if (TargetActorClass == nullptr)
	{
		return;
	}

	// 충돌 AT
	if (UMyATWeaponHit* WeaponTraceTask = UMyATWeaponHit::CreateTask(this, TargetActorClass))
	{
		WeaponTraceTask->OnComplete.AddDynamic(this, &UMyGAGrabSword::OnTraceResultCallback);
		WeaponTraceTask->ReadyForActivation();
	}
}

void UMyGAGrabSword::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		return;
	}

	// 무기
	AActor* Sword = nullptr;

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		const FHitResult HR = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		Sword = HR.GetActor();
	}

	if (!Sword && TargetDataHandle.Data.Num() > 0)
	{
		if (const FGameplayAbilityTargetData* TD = TargetDataHandle.Data[0].Get())
		{
			if (TD->GetActors().Num() > 0)
			{
				Sword = TD->GetActors()[0].Get();
			}
		}
	}

	if (!Sword)
	{
		return;
	}

	USkeletalMeshComponent* CharMesh = Character->GetMesh();
	if ((!CharMesh) || (!CharMesh->DoesSocketExist(HandSocketName)))
	{
		return;
	}

	// 무기 장착 셋팅
	if (USkeletalMeshComponent* WeaponSMC = Sword->FindComponentByClass<USkeletalMeshComponent>())
	{
		WeaponSMC->SetSimulatePhysics(false);
		WeaponSMC->SetEnableGravity(false);
		WeaponSMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Sword->SetOwner(Character);
	Sword->SetInstigator(Character);
	// 무기 장착
	Sword->AttachToComponent(CharMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocketName);

	// 무기 장착 태그
	if (HasWeaponTag.IsValid())
	{
		if (UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked())
		{
			SourceASC->AddLooseGameplayTag(HasWeaponTag);
		}
	}

	if (UBoxComponent* Collision = Sword->FindComponentByClass<UBoxComponent>())
	{
		Collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	}

	AMyPlayerController* Controller = Cast<AMyPlayerController>(Character->GetController());
	if (Controller)
	{
		Controller->EquipmentWeapon();
	}
}

void UMyGAGrabSword::OnMontageCompleted()
{
	if (WaitEventTask)
	{
		WaitEventTask->EndTask();
		WaitEventTask = nullptr;
	}

	const bool bReplicatedEndAbility = true;
	const bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMyGAGrabSword::OnMontageInterruptedOrCancelled()
{
	if (WaitEventTask)
	{
		WaitEventTask->EndTask();
		WaitEventTask = nullptr;
	}

	const bool bReplicatedEndAbility = true;
	const bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}