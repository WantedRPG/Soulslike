// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MyATJumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyATJumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UMyATJumpAndWaitForLanding() = default;

public:
	// AbilityTask 생성
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UMyATJumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

protected:
	// AbilityTask 활성화 시 호출
	virtual void Activate() override;
	// AbilityTask 종료 시 호출
	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	// 캐릭터 착지 콜백 함수
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);

public:
	// 착지 시 브로드 캐스트 함수
	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForLandingDelegate OnComplete;
};
