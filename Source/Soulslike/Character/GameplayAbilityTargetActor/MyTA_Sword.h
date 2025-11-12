// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "MyTA_Sword.generated.h"

UCLASS()
class SOULSLIKE_API AMyTA_Sword : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AMyTA_Sword() = default;

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool bInShow) { bShowDebug = bInShow; }

protected:
	FGameplayAbilityTargetDataHandle MakeTargetData() const;
	bool bShowDebug = false;

protected:
	// TODO. 무기 충돌 채널 및 태그 변경하기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Collision")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_WorldDynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting|Pick")
	FName RequiredActorTag = FName(TEXT("Weapon"));
};
