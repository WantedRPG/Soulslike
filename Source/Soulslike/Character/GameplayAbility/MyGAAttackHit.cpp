// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbility/MyGAAttackHit.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/AbilityTask/MyATAttackHit.h"
#include "Character/GameplayAbilityTargetActor/MyTA_Target.h"
#include "Character/Tag/MyGameplayTag.h"
#include "AttributeSet/SLAttributeSet.h"

UMyGAAttackHit::UMyGAAttackHit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyGAAttackHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 노티파이로부터 공격 레벨 획득
	CurrentLevel = TriggerEventData->EventMagnitude;
	AttackTag = TriggerEventData->EventTag;

	UMyATAttackHit* AttackTraceTask = UMyATAttackHit::CreateTask(this, AMyTA_Target::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UMyGAAttackHit::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UMyGAAttackHit::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 피격 판정 존재 확인
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogTemp, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		// 몬스터 정보 셋팅
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		const USLAttributeSet* SourceAttribute = SourceASC->GetSet<USLAttributeSet>();

		// 공격력만 셋팅된 GE이므로 데미지만 적용됨. 
		TSubclassOf<UGameplayEffect> AttackEffect = EffectByAttackTag.FindRef(AttackTag);
		if (AttackEffect)
		{
			FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(AttackEffect, CurrentLevel);

			if (Spec.IsValid())
			{
				// TODO. 여기서 몬스터한테 레벨값(스킬별 공격력) 반영하기. 
				// (1) GE 작업해야 함. AttackRate를 적용하는 방식.
				// (2) 다만 현재 로직은 AttributeSet의 AttackPower 자체를 바꾸는 게 아님.... Set하고 나서 Get로 전달해주는 방향으로 수정
				Spec.Data->SetSetByCallerMagnitude(MyTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
				// GE로 전달
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Spec, TargetDataHandle);
			}
		}

	}

	// 종료
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
