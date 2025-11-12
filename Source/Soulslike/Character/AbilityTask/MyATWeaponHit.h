// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MyATWeaponHit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyATWeaponHit : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UMyATWeaponHit() = default;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "WaitForWeaponTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UMyATWeaponHit* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AMyTA_Sword> TargetActorClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FWeaponTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AMyTA_Sword> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AMyTA_Sword> SpawnedTargetActor;
};
