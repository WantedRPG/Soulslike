// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MyATAttackHit.generated.h"

// 공격 판정 결과 전달 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

// 공격 판정 AT
UCLASS()
class SOULSLIKE_API UMyATAttackHit : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UMyATAttackHit() = default;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "WaitForAttackTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UMyATAttackHit* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AMyTA_Target> TargetActorClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	// 데이터 수신 콜백
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	// TA 클래스
	UPROPERTY()
	TSubclassOf<class AMyTA_Target> TargetActorClass;

	// TA 액터
	UPROPERTY()
	TObjectPtr<class AMyTA_Target> SpawnedTargetActor;
};
