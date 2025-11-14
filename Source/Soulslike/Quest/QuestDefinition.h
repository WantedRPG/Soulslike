// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestData.h"
#include "QuestDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULSLIKE_API UQuestDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Default constructor for the class
	UQuestDefinition();

public:
	// The ID name of this quest for referencing in a table row
	UPROPERTY(EditAnywhere, Category = "Quest Data")
	FName ID;

	// The type of the quest
	UPROPERTY(EditAnywhere, Category = "Quest Data")
	EQuestType Type;

	// Text struct including the quest name and description
	UPROPERTY(EditAnywhere, Category = "Quest Data")
	FQuestText Text;
};
