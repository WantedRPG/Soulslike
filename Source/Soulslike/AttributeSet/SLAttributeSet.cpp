#include "AttributeSet/SLAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Tag/MyGameplayTag.h"
#include "Monster/Common/SLMonsterbase.h"

USLAttributeSet::USLAttributeSet()
{
}

void USLAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);
}

void USLAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
    if (AMyPlayer* Player = Cast<AMyPlayer>(TargetActor))
    {
        if (Data.EvaluatedData.Attribute == GetHealthAttribute())
        {
            if (Data.Target.AbilityActorInfo.IsValid())
            {
                if (Data.Target.HasMatchingGameplayTag(MyTAG_Invincibility))
                {
                    SetAttackPower(0.0f);
                    UE_LOG(LogTemp, Warning, TEXT("무적 상태 구르기. 데미지 없음."));
                    return;
                }

                if (GetHealth() > 0.f)
                {
                    const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();
                    Player->KnockBack(Context, Data.EvaluatedData.Magnitude);
                }
                else
                {
                    SetHealth(0.f);
                    Player->Death();
                }
            }
        }

        if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
        {
            if (GetStamina() > 0.f)
            {
                SetStamina(FMath::Clamp(GetStamina(), 1.f, 50.f));
            }
            else
            {
                const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();
                Player->StopSprint(Context);
                SetStamina(0.f);
            }
        }
    }
    if (ASLMonsterbase* Monster = Cast<ASLMonsterbase>(TargetActor))
    {
        if (Data.EvaluatedData.Attribute == GetHealthAttribute())
        {
            if (GetHealth() <= 0.f)
            {
                Monster->MonsterDead();
                SetHealth(0.0f);
            }
        }
    }
}