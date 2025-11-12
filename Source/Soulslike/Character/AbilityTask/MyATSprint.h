// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MyATSprint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSprintDelegate);

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyATSprint : public UAbilityTask
{
	GENERATED_BODY()

public:
	UMyATSprint();

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "Sprint", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UMyATSprint* CreateTask(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	UFUNCTION()
	void OnSprintCallback(const FHitResult& Hit);

public:
	UPROPERTY(BlueprintAssignable)
	FSprintDelegate OnComplete;

protected:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float DefaultMovementSpeed = 600;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintMovementSpeed = 2000;
};
