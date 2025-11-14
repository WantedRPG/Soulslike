// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/SLAttributeSet.h"
#include "GameplayEffectExtension.h"

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

	if (Attribute == GetAttackPowerAttribute()) 
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

void USLAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) 
	{
		UE_LOG(LogTemp, Log, TEXT("Health : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	
	if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute()) 
	{
		UE_LOG(LogTemp, Log, TEXT("Damage : %f"), GetAttackPower());
		// 데미지를 체력에 반영
		SetHealth(FMath::Clamp(GetHealth() - GetAttackPower(), 0.f, GetMaxHealth()));
		SetAttackPower(0.0f);
	}
}

//
//void USLAttributeSet::PostGameplayEffectExecute(const FGameplayAttribute& Attribute, float& OldValue, float& NewValue)
//{
//	// Super::PostGameplayEffectExecute(Attribute, OldValue, NewValue);
//
//	// 데미지가 체력에 반영됐는지 확인하는 코드
//	if (Attribute == GetHealthAttribute()) 
//	{
//		UE_LOG(LogTemp, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
//	}
//}
