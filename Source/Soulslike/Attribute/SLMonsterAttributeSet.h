#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SLMonsterAttributeSet.generated.h"

// 접근자 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SOULSLIKE_API USLMonsterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USLMonsterAttributeSet();

	// 공통
	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData Health;              // 현재체력
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData MaxHealth;           // 최대체력
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData AttackPower;         // 공격력
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, AttackPower);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData MoveSpeed;           // 이동속도
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData PoiseResist;         // 경직저항
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, PoiseResist);

	// 화염
	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData FireResist;          // 화염저항(0~1)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, FireResist);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData OverheatResist;      // 과열 상태이상 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, OverheatResist);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData OverheatBuildup;     // 과열 누적치(0~100)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, OverheatBuildup);

	// 전격
	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData ElectricResist;      // 전격저항(0~1)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, ElectricResist);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData ShockResist;         // 감전 상태이상 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, ShockResist);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData ShockBuildup;        // 감전 누적치(0~100)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, ShockBuildup);

	// 산성
	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData AcidResist;          // 산성저항(0~1)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, AcidResist);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData CorrodeResist;       // 부식 상태이상 저항(0~1)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, CorrodeResist);

	UPROPERTY(BlueprintReadOnly, Category = "MonsterAttribute")
	FGameplayAttributeData CorrodeBuildup;      // 부식 누적치(0~100)
	ATTRIBUTE_ACCESSORS(USLMonsterAttributeSet, CorrodeBuildup);

	// 값이 실제로 바뀌기 직전에 한 번 호출
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// GameplayEffect 후에 호출
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
};
