// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Objective.h"
#include "Quest.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SOULSLIKE_API UQuest : public UObject
{
	GENERATED_BODY()

public:
	UQuest();

	UPROPERTY(EditAnywhere, Category = Quest)
	FName ID;

	// Quest summary
	UPROPERTY(EditAnywhere, Category = Quest)
	FText Summary;

	// Quest description for detail
	UPROPERTY(EditAnywhere, Category = Quest)
	FText Description;

	// Objectives of a quest
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<TObjectPtr<AObjective>> Objectives;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Quest)
	bool QuestScript();
};