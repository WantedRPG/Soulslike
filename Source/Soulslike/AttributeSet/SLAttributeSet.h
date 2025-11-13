// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SLAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 플레이어 및 몬스터의 기본 능력치를 정의하는 AttributeSet 클래스
 */
UCLASS()
class SOULSLIKE_API USLAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	USLAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Health);			// 체력

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MaxHealth);		// 최대 체력

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | MovementSpeed", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MovementSpeed);	// 이동 속도

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | MovementSpeed", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MaxMovementSpeed); // 최대 이동 속도

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Stamina);			// 현재 스태미나

#pragma region Resist
	UPROPERTY(BlueprintReadOnly, Category = "Resist | PoiseResist", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Resist_Poise;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Resist_Poise);		 // 경직 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "Resist | Acid")
	FGameplayAttributeData Resist_Acid;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Resist_Acid);		 // 산성 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "Resist | Fire")
	FGameplayAttributeData Resist_Fire;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Resist_Fire);		 // 화염 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "Resist | Electricity")
	FGameplayAttributeData Resist_Electricity;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Resist_Electricity); // 전격 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "Resist | Impact")
	FGameplayAttributeData Resist_Impact;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Resist_Impact);	  // 충격 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "Resist | Breakdown")
	FGameplayAttributeData Resist_Breakdown;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Resist_Breakdown);   // 붕괴 저항 수치
#pragma endregion

// TODO. 누적치가 임계값을 넘이면 상태 이상 이벤트 발생. 이는 GE의 Level 변경으로 활용할 예정.
#pragma region StatusResist
	UPROPERTY(BlueprintReadOnly, Category = "StatusResist | Acid")
	FGameplayAttributeData StatusResist_Corrode;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, StatusResist_Corrode);  //	부식 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "StatusResist | Fire")
	FGameplayAttributeData StatusResist_Overheat;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, StatusResist_Overheat); // 과열 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "StatusResist | Electricity")
	FGameplayAttributeData StatusResist_Shock;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, StatusResist_Shock);    // 감전 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "StatusResist | Impact")
	FGameplayAttributeData StatusResist_Collapse;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, StatusResist_Collapse); // 충격 저항 수치

	UPROPERTY(BlueprintReadOnly, Category = "StatusResist | Breakdown")
	FGameplayAttributeData StatusResist_Death;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, StatusResist_Death);   // 붕괴 저항 수치
#pragma endregion

// 상태 이상 상황인지 판단할, 누적치
#pragma region Bulidup
	UPROPERTY(BlueprintReadOnly, Category = "Bulidup | Acid")
	FGameplayAttributeData Buildup_Corrode;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Buildup_Corrode);		 // 산성 누적치

	UPROPERTY(BlueprintReadOnly, Category = "Bulidup | Fire")
	FGameplayAttributeData Bulidup_Overheat;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Bulidup_Overheat);		 // 화염 누적치

	UPROPERTY(BlueprintReadOnly, Category = "Bulidup | Electricity")
	FGameplayAttributeData Buildup_Shock;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Buildup_Shock);		// 전격 누적치

	UPROPERTY(BlueprintReadOnly, Category = "Bulidup | Impact")
	FGameplayAttributeData Buildup_Collapse;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Buildup_Collapse);	    // 충격 누적치

	UPROPERTY(BlueprintReadOnly, Category = "Bulidup | Breakdown")
	FGameplayAttributeData Buildup_Death;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Buildup_Death);		// 붕괴 누적치
#pragma endregion

	// Notify 발동할 때마다, 공격력만 다르게 적용.
	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Attack Power", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, AttackPower);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Attack Rate", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, AttackRate);

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

};
