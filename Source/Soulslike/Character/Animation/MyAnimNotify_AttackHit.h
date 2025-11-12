// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "MyAnimNotify_AttackHit.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyAnimNotify_AttackHit : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UMyAnimNotify_AttackHit();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;

	UPROPERTY(EditAnywhere)
	float ComboAttackLevel;
};
