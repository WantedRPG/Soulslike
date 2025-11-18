// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestData.generated.h"

class UQuestDefinition;

UENUM()
enum class EQuestType : uint8
{
	Main UMETA(DisplayName = "MainQuest"),
	Side UMETA(DisplayName = "SideQuest")
};

USTRUCT()
struct FQuestText
{
	GENERATED_BODY()

	// The text name of the quest
	UPROPERTY(EditAnywhere)
	FText Name;

	// The text description fo the quest
	UPROPERTY(EditAnywhere)
	FText Description;
};

USTRUCT()
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()

	// The ID name of this quest for referencing in a table row
	UPROPERTY(EditAnywhere, Category = "Quest Data")
	FName ID;

	// The type of the quest
	UPROPERTY(EditAnywhere, Category = "Quest Data")
	EQuestType Type;

	// Text struct including the quest name and description
	UPROPERTY(EditAnywhere, Category = "Quest Data")
	FQuestText Text;

	// The Data Asset quest definition associated with this quest
	UPROPERTY(EditAnywhere, Category = "Quest Data")
	TObjectPtr<UQuestDefinition> QuestDataAsset;
};