// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbility/MyGAAttackHit.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/AbilityTask/MyATAttackHit.h"
#include "Character/GameplayAbilityTargetActor/MyTA_Target.h"
#include "Character/Tag/MyGameplayTag.h"
#include "AttributeSet/SLAttributeSet.h"
#include "Monster/Common/SLMonsterbase.h"

UMyGAAttackHit::UMyGAAttackHit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyGAAttackHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 노티파이로부터 공격 레벨 획득
	CurrentLevel = TriggerEventData->EventMagnitude;
	// AttackTag = TriggerEventData->EventTag;

	UMyATAttackHit* AttackTraceTask = UMyATAttackHit::CreateTask(this, AMyTA_Target::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UMyGAAttackHit::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UMyGAAttackHit::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMyGAAttackHit::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
    if (!UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }

    // 몬스터 정보 셋팅
    FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
    if (!HitResult.bBlockingHit)
    {
        return;
    }

    ASLMonsterbase* Monster = Cast<ASLMonsterbase>(HitResult.GetActor());
    if (!Monster)
    {
        return;
    }

    UAbilitySystemComponent* MonsterASC = Monster->FindComponentByClass<UAbilitySystemComponent>();
    if (!MonsterASC)
    {
        return;
    }

    // 플레이어 정보 셋팅
    UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
    if (!SourceASC)
    {
        return;
    }

    // 공격 스탯
    const float AttackPower = SourceASC->GetNumericAttribute(USLAttributeSet::GetAttackPowerAttribute());

    FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
    Context.AddSourceObject(GetAvatarActorFromActorInfo());

    // 데미지 이펙트 적용
    FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(AttackDamageEffect, CurrentLevel, Context);
    if (!SpecHandle.IsValid())
    {
        return;
    }

    // 데미지 전달
    MonsterASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	// 종료
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
