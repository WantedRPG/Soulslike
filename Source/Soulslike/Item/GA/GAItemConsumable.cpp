// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/GA/GAItemConsumable.h"

void UGAItemConsumable::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // 1. 유효성 검사
    if (!ActorInfo->AbilitySystemComponent.IsValid())
    {
        Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    
    /*
    // ItemData.HealAmount = 50; // 예시 값

    // 3. 적용할 GE 클래스 가져오기 (블루프린트에서 설정한 TSubclassOf<UGEBase> 사용)
    TSubclassOf<UGameplayEffect> EffectToApply = ItemData.ItemEffectClass;

    if (EffectToApply == nullptr)
    {
        Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 4. GameplayEffectSpec 생성
    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectToApply, 1.0f, ContextHandle);

    if (SpecHandle.IsValid())
    {
        // 5. SetByCaller를 사용하여 동적 값 주입 (핵심!)
        // 블루프린트에서 설정한 태그(HealMagnitudeTag)와 아이템 데이터의 값(HealAmount) 연결
        SpecHandle.Data.Get()->SetSetByCallerMagnitude(HealMagnitudeTag, (float)ItemData.HealAmount);

        // 6. GE 적용
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

    // 7. 어빌리티 종료
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
    */
}
