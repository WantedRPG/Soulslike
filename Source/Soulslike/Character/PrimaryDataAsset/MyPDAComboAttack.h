// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyPDAComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyPDAComboAttack : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UMyPDAComboAttack() = default;

public:
	UPROPERTY(EditAnywhere, Category = "Melee Attack | Combo")
	TArray<FName> ComboSectionNames;

	UPROPERTY(EditAnywhere, Category = "Melee Attack | ComboFrame")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "Melee Attack | ComboFrame")
	TArray<float> CheckComboFrameCount;
};
