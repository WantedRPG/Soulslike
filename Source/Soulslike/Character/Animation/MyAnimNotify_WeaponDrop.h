// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "MyAnimNotify_WeaponDrop.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyAnimNotify_WeaponDrop : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;
};
