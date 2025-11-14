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

#pragma region Default
	// 체력
	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MaxHealth);

	// 이동 속도
	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | MovementSpeed", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | MovementSpeed", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MaxMovementSpeed);

	// 공격력
	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Attack Power", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Power;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Power);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Attack Rate", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, AttackRate);

	// 현재 스태미나
	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Stamina);
	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MaxStamina);


	// 경직 저항 수치
	UPROPERTY(BlueprintReadOnly, Category = "SLAttributeSet | Resist", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PoiseResist;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, PoiseResist);
#pragma endregion

#pragma region Skill
	// 화염
	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Flame")
	FGameplayAttributeData FireResist;          // 화염저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, FireResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Flame")
	FGameplayAttributeData OverheatResist;      // 과열 상태이상 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, OverheatResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Flame")
	FGameplayAttributeData OverheatBuildup;     // 과열 누적치(0~100)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, OverheatBuildup);

	// 전격
	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Electricity")
	FGameplayAttributeData ElectricResist;      // 전격저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, ElectricResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Electricity")
	FGameplayAttributeData ShockResist;         // 감전 상태이상 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, ShockResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Electricity")
	FGameplayAttributeData ShockBuildup;        // 감전 누적치(0~100)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, ShockBuildup);

	// 산성
	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Acid")
	FGameplayAttributeData AcidResist;          // 산성저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, AcidResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Acid")
	FGameplayAttributeData CorrodeResist;       // 부식 상태이상 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, CorrodeResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Acid")
	FGameplayAttributeData CorrodeBuildup;      // 부식 누적치(0~100)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, CorrodeBuildup);

	// 붕괴
	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Breakdown")
	FGameplayAttributeData BreakdownResist;     // 붕괴저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, BreakdownResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Breakdown")
	FGameplayAttributeData DeathResist;         // 즉사 상태이상 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, DeathResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Breakdown")
	FGameplayAttributeData DeathBuildup;        // 즉사 누적치(0~100)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, DeathBuildup);

	// 충격
	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Impact")
	FGameplayAttributeData ImpactResist;        // 충격 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, ImpactResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Impact")
	FGameplayAttributeData CollapseResist;       // 충격 상태이상 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, CollapseResist);

	UPROPERTY(BlueprintReadOnly, Category = "SLAttribute | Impact")
	FGameplayAttributeData CollapseBuildup;      // 부식 누적치(0~100)
	ATTRIBUTE_ACCESSORS(USLAttributeSet, CollapseBuildup);

#pragma endregion

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

};
