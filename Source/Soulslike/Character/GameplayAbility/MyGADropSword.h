// MyGADropSword.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "MyGADropSword.generated.h"

UCLASS()
class SOULSLIKE_API UMyGADropSword : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMyGADropSword();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnNotifyEventReceived(FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterruptedOrCancelled();

	void DoDropNow();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop | Montage")
	TObjectPtr<UAnimMontage> DropMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop | AnimNotify")
	FGameplayTag DropNotifyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	FName HandSocketName = TEXT("hand_rSocket");

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask = nullptr;
};

