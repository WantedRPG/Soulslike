#include "AttributeSet/SLAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Character/Player/MyPlayer.h"
#include "AbilitySystemComponent.h"

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
                if (GetHealth() <= 0.f)
                {
                    Player->Death();
                }
                else
                {
                    const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();
                    Player->KnockBack(Context);
                }
            }
        }

        if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
        {
            //static const FGameplayTag RollTag = FGameplayTag::RequestGameplayTag(TEXT("Character.State.Roll"));
            //if (Data.Target.AbilityActorInfo.IsValid())
            //{
            //    if (UAbilitySystemComponent* PlayerASC = Data.Target.AbilityActorInfo->AbilitySystemComponent.Get())
            //    {
            //        FGameplayTagContainer OwnedTags;
            //        PlayerASC->GetOwnedGameplayTags(OwnedTags);

            //        // 구르기 무적 태그가 있으면 데미지 무시
            //        if (OwnedTags.HasTagExact(RollTag))
            //        {
            //            UE_LOG(LogTemp, Warning, TEXT("무적 구르기. 데미지 무시"));
            //            SetAttackPower(0.0f);
            //            return;
            //        }
            //    }
            //}

            SetHealth(FMath::Clamp(GetHealth() - GetAttackPower(), 0.f, GetMaxHealth()));
            SetAttackPower(0.0f);
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