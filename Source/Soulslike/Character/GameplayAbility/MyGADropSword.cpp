// MyGADropSword.cpp

#include "Character/GameplayAbility/MyGADropSword.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"

#include "AbilitySystemComponent.h"  
#include <Components/BoxComponent.h>
#include <Character/Player/MyPlayerController.h>

UMyGADropSword::UMyGADropSword()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyGADropSword::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	if (DropNotifyTag.IsValid())
	{
		WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, DropNotifyTag, nullptr, /*OnlyOnce*/false, /*Exact*/true);
		if (WaitEventTask)
		{
			WaitEventTask->EventReceived.AddDynamic(this, &UMyGADropSword::OnNotifyEventReceived);
			WaitEventTask->ReadyForActivation();
		}
	}

	if (DropMontage)
	{
		if (UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, TEXT("DropMontage"), DropMontage, 1.f/*rate*/, TEXT("Drop"), true, 1.f))
		{
			MontageTask->OnCompleted.AddDynamic(this, &UMyGADropSword::OnMontageCompleted);
			MontageTask->OnInterrupted.AddDynamic(this, &UMyGADropSword::OnMontageInterruptedOrCancelled);
			MontageTask->OnCancelled.AddDynamic(this, &UMyGADropSword::OnMontageInterruptedOrCancelled);
			MontageTask->ReadyForActivation();
			return;
		}
	}

	if (!DropMontage && !DropNotifyTag.IsValid())
	{
		DoDropNow();
		OnMontageCompleted();
	}
}

void UMyGADropSword::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AMyPlayer* MyPlayer = CastChecked<AMyPlayer>(ActorInfo->AvatarActor.Get());
	MyPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UMyGADropSword::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMyGADropSword::OnNotifyEventReceived(FGameplayEventData /*EventData*/)
{
	DoDropNow();
}

void UMyGADropSword::DoDropNow()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character) return;

	USkeletalMeshComponent* CharMesh = Character->GetMesh();
	if (!CharMesh) return;

	// TODO. 추후 인벤토리랑
	TArray<AActor*> AttachedActors;
	Character->GetAttachedActors(AttachedActors);

	AActor* FoundSword = nullptr;
	for (AActor* Attached : AttachedActors)
	{
		if (!Attached) continue;

		if (USceneComponent* RootComp = Attached->GetRootComponent())
		{
			if (RootComp->GetAttachSocketName() == HandSocketName)
			{
				FoundSword = Attached;
				break;
			}
		}
	}

	if (!FoundSword) return;

	if (UBoxComponent* Collision = FoundSword->FindComponentByClass<UBoxComponent>())
	{
		Collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); //WorldDynamic
	}

	AMyPlayerController* Controller = Cast<AMyPlayerController>(Character->GetController());
	if (Controller)
	{
		Controller->UnequipmentWeapon();
	}

	// 땅에 드랍
	if (USceneComponent* RootComp = FoundSword->GetRootComponent())
	{
		RootComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		if (UStaticMeshComponent* SMC = FoundSword->FindComponentByClass<UStaticMeshComponent>())
		{
			SMC->SetSimulatePhysics(true);
			SMC->SetEnableGravity(true);
			SMC->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(RootComp))
		{
			Prim->SetSimulatePhysics(true);
			Prim->SetEnableGravity(true);
			Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}

		FoundSword->SetOwner(nullptr);
		FoundSword->SetInstigator(nullptr);

		// 무기 장착 태그 해제
		if (HasWeaponTag.IsValid())
		{
			if (UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked())
			{
				SourceASC->RemoveLooseGameplayTag(HasWeaponTag);
			}
		}
	}
}

void UMyGADropSword::OnMontageCompleted()
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

void UMyGADropSword::OnMontageInterruptedOrCancelled()
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
