// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Objective.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SOULSLIKE_API AObjective : public AActor
{
	GENERATED_BODY()

public:
	AObjective();

	UPROPERTY(EditAnywhere, Category = Quest)
	FString Name;

	UPROPERTY(EditAnywhere, Category = Quest)
	FText Description;




};
