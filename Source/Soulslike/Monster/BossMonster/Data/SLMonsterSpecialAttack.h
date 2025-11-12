// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "SLMonsterSpecialAttack.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLMonsterSpecialAttack : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> SpecialAttacks;
};
