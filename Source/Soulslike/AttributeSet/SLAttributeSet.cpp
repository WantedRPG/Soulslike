
#include "AttributeSet/SLAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Character/Player/MyPlayer.h"

USLAttributeSet::USLAttributeSet()
{
}

void USLAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) 
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
}

void USLAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) 
	{
		float NewHealth = GetHealth();
		float OldHealth = NewHealth - Data.EvaluatedData.Magnitude;

		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		NewHealth = GetHealth();

		if (OldHealth > NewHealth)
		{
			if (Data.Target.AbilityActorInfo.IsValid())
			{
				AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
				if (AMyPlayer* Player = Cast<AMyPlayer>(TargetActor))
				{
					if (NewHealth <= 0.f)
					{
						Player->Death();
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("KnockBack"));
						const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();
						Player->KnockBack(Context);
					}
				}
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute()) 
	{
		UE_LOG(LogTemp, Log, TEXT("Damage : %f"), GetAttackPower());
		// 데미지를 체력에 반영
		SetHealth(FMath::Clamp(GetHealth() - GetAttackPower(), 0.f, GetMaxHealth()));
		SetAttackPower(0.0f);
	}
}
