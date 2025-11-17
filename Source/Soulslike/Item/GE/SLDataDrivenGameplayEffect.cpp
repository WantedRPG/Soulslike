// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/GE/SLDataDrivenGameplayEffect.h"

USLDataDrivenGameplayEffect::USLDataDrivenGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
    /*
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UYourAttributeSet::GetHealthAttribute(); // Health Attribute 타겟팅
    ModifierInfo.ModifierOp = EGameplayModifierOp::Additive;           // 더하기 연산 (회복)

    // SetByCaller 설정을 위한 Magnitude 정의
    FGameplayEffectModifierMagnitude Magnitude;
    Magnitude.MagnitudeCalculationType = EGameplayEffectMagnitudeCalculation::SetByCaller;

    // 사용할 태그 지정 ("Data.HealingValue" 태그는 프로젝트 설정에 미리 추가되어 있어야 합니다)
    // 태그가 없을 경우 런타임에 RequestGameplayTag로 생성도 가능합니다.
    FGameplayTag HealingTag = UGameplayTagsManager::Get().RequestGameplayTag(TEXT("Data.HealingValue"));
    Magnitude.SetDataTag(HealingTag);

    // 기본값 설정 (만약 값이 전달되지 않으면 0 회복)
    Magnitude.SetScalableFloatMagnitude(FScalableFloat(0.0f));

    ModifierInfo.Magnitude = Magnitude;
    Modifiers.Add(ModifierInfo);

    */
}
