// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/SLAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Character/Player/MyPlayer.h"

// 몬스터 데미지 반영을 위해 임시로 0 이하로 체력이 떨어질 수 있도록 수정함.
// Player의 State를 반영하는 로직을 임시로 반영.

USLAttributeSet::USLAttributeSet()
{
}

void USLAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	/*if (Attribute == GetHealthAttribute()) 
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}*/

	/*if (Attribute == GetAttackPowerAttribute()) 
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}*/
}

void USLAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) 
	{
		UE_LOG(LogTemp, Log, TEXT("Health : %f"), GetHealth());
		//bSetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		SetHealth(GetHealth());
	}
	
	if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute()) 
	{
		UE_LOG(LogTemp, Log, TEXT("Damage : %f"), GetAttackPower());
		// 데미지를 체력에 반영
		// SetHealth(FMath::Clamp(GetHealth() - GetAttackPower(), 0.f, GetMaxHealth()));
		SetHealth(GetHealth() - GetAttackPower());
		SetAttackPower(0.0f);

		// 죽음 처리
		if (Data.Target.AbilityActorInfo.IsValid())
		{
			AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			if (AMyPlayer* Player = Cast<AMyPlayer>(TargetActor))
			{
				if (GetHealth() <= 0.f)
				{
					Player->Death();
				}
			}
		}
	}
}
