// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestData.h"
#include "QuestDefinition.generated.h"

class AQuestObjectiveActor;
class UQuestObjectiveDefinition;

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

	// Objective actor classes associated with this quest.
	// 디자이너는 여기서 Objective Actor의 블루프린트 클래스를 지정할 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
	TSubclassOf<AQuestObjectiveActor> ObjectiveActorClass;

	// Quest objective 데이터 자산들 (각 Objective의 세부 정의)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
	TArray<TObjectPtr<UQuestObjectiveDefinition>> ObjectiveDefinitions;
};
