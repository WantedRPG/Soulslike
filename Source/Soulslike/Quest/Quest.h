// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Objective.h"
#include "Quest.generated.h"


UENUM()
enum class EQuestType : uint8
{
	Main,
	Side,
	Arbeit
};

/**
 * 
 */
UCLASS()
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

	// Quest type
	UPROPERTY(EditAnywhere, Category = Quest)
	EQuestType Type = EQuestType::Main;

	// Objectives of a quest
	UPROPERTY(EditAnywhere, Category = Quest)
	TArray<TObjectPtr<UObjective>> Objectives;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Quest)
	bool QuestScript();
};