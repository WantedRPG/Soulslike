// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Objective.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UObjective : public UObject
{
	GENERATED_BODY()

public:
	UObjective();

	UPROPERTY(EditAnywhere, Category = Quest)
	FString Name;

	UPROPERTY(EditAnywhere, Category = Quest)
	FText Description;


};
