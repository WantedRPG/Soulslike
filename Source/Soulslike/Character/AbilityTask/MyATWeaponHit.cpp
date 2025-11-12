// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AbilityTask/MyATWeaponHit.h"
#include "AbilitySystemComponent.h"
#include "Character/GameplayAbilityTargetActor/MyTA_Sword.h"

UMyATWeaponHit* UMyATWeaponHit::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AMyTA_Sword> TargetActorClass)
{
	UMyATWeaponHit* NewTask = NewAbilityTask<UMyATWeaponHit>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UMyATWeaponHit::Activate()
{
	Super::Activate();

	// TA 스폰
	SpawnAndInitializeTargetActor();
	// TA 스폰 마무리
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UMyATWeaponHit::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UMyATWeaponHit::SpawnAndInitializeTargetActor()
{
	// TA 액터 스폰
	SpawnedTargetActor = Cast<AMyTA_Sword>(
		Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(
			TargetActorClass,
			FTransform::Identity,
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	if (SpawnedTargetActor)
	{
		// 디버그
		SpawnedTargetActor->SetShowDebug(true);
		// 콜백 등록
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UMyATWeaponHit::OnTargetDataReadyCallback);
	}
}

void UMyATWeaponHit::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		// ASC에 기록
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UMyATWeaponHit::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates()/*TA → AT*/)
	{
		OnComplete.Broadcast(DataHandle); // AT → GA
	}

	EndTask();
}