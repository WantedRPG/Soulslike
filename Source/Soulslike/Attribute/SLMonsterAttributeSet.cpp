#include "SLMonsterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"

USLMonsterAttributeSet::USLMonsterAttributeSet()
{
	// 기본값(디폴트값)
	InitMaxHealth(100.f);
	InitHealth(100.f);
	InitAttackPower(10.f);
	InitMoveSpeed(450.f);
	InitPoiseResist(0.0f);

	InitFireResist(0.0f);
	InitOverheatResist(0.0f);
	InitOverheatBuildup(0.0f);

	InitElectricResist(0.0f);
	InitShockResist(0.0f);
	InitShockBuildup(0.0f);

	InitAcidResist(0.0f);
	InitCorrodeResist(0.0f);
	InitCorrodeBuildup(0.0f);
}

void USLMonsterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attr, float& NewValue)
{
}

void USLMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
}