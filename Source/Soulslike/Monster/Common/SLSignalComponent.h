// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SLSignalComponent.generated.h"

// GA 종료 신호: 어떤 GA가 끝났는지 GameplayTag로 전달
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FSLAbilityEndedSignature,
	FGameplayTag, AbilityTag
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API USLSignalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USLSignalComponent();

protected:

public:	
	UPROPERTY(BlueprintAssignable, Category = "Signal")
	FSLAbilityEndedSignature OnAbilityEnded;


	// 브로드캐스트
	UFUNCTION(BlueprintCallable, Category = "Signal")
	void BroadcastAbilityEnded(const FGameplayTag& AbilityTag)
	{
		OnAbilityEnded.Broadcast(AbilityTag);
	}
};
