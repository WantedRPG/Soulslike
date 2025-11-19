#include "AttributeSet/SLAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Tag/MyGameplayTag.h"

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
        if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
        {
            SetHealth(FMath::Clamp(GetHealth() - GetAttackPower(), 0.f, GetMaxHealth()));
            SetAttackPower(0.0f);
        }

        if (Data.EvaluatedData.Attribute == GetHealthAttribute())
        {
            if (Data.Target.AbilityActorInfo.IsValid())
            {
                if (Data.Target.HasMatchingGameplayTag(MyTAG_Invincibility))
                {
                    SetAttackPower(0.0f);
                    //const float NewHealth = GetHealth();
                    //const float Mag = Data.EvaluatedData.Magnitude; // 공격력 : -30
                    //const float OriginalHealth = NewHealth - Mag;   // 원래 체력 : 70 -(-30) = 100
                    //SetHealth(OriginalHealth);
                    UE_LOG(LogTemp, Warning, TEXT("무적 상태 구르기. 데미지 없음."));
                    return;
                }

                if (GetHealth() > 0.f)
                {
                    const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();

					const float KnockBacKPower = Data.EvaluatedData.Magnitude;
                    int32 KnockBackLevel = (KnockBacKPower <= 50.f) ? 1 : 2;

                    Player->KnockBack(Context, KnockBackLevel);  
                }
                else
                {
                    Player->Death();
                    SetHealth(0.f);
                }
            }
        }


        if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
        {
            const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();

            if (GetStamina() <= 0.f)
            {
                Player->StopSprint(Context);
                SetStamina(0.f);
            }
            else
            {
                SetStamina(FMath::Clamp(GetStamina(), 0.f, 50.f));
            }
        }
    }
}