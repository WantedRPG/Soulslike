// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AbilityTask/MyATAttackHit.h"
#include "AbilitySystemComponent.h"
#include "Character/GameplayAbilityTargetActor/MyTA_Target.h"

UMyATAttackHit* UMyATAttackHit::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AMyTA_Target> TargetActorClass)
{
	UMyATAttackHit* NewTask = NewAbilityTask<UMyATAttackHit>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UMyATAttackHit::Activate()
{
	Super::Activate();

	// TA 스폰
	SpawnAndInitializeTargetActor();
	// TA 스폰 마무리
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UMyATAttackHit::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UMyATAttackHit::SpawnAndInitializeTargetActor()
{
	// TA 액터 스폰
	SpawnedTargetActor = Cast<AMyTA_Target>(
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
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UMyATAttackHit::OnTargetDataReadyCallback);
	}
}

void UMyATAttackHit::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		// TA 액터 스폰
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		// ASC에 기록
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		// TA 공격 판정 시작
		SpawnedTargetActor->StartTargeting(Ability);
		// TA 공격 판정 결과 기록
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UMyATAttackHit::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle) 
{
	if (ShouldBroadcastAbilityTaskDelegates()/*TA → AT*/)
	{
		OnComplete.Broadcast(DataHandle); // AT → GA
	}

	EndTask();
}