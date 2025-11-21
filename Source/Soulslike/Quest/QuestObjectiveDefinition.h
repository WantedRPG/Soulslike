// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestObjectiveDefinition.generated.h"

class USLItemData;

/**
 * 
 */

// Objective 타입을 이 파일로 이동
UENUM()
enum class EQuestObjectiveType : uint8
{
	GetItem UMETA(DisplayName = "GetItem"),
	MoveToDest UMETA(DisplayName = "MoveToDestination")
};

UCLASS()
class SOULSLIKE_API UQuestObjectiveDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FText ObjectiveName;

	// Spawn 위치(레벨 내 Transform). 에디터/블루프린트에서 변경 가능.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FTransform SpawnTransform;

	// Objective 타입 (GetItem, Arrival 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	EQuestObjectiveType ObjectiveType = EQuestObjectiveType::MoveToDest;

	// GetItem 타입인 경우 요구되는 아이템을 에디터에서 지정 (USLItemData의 서브클래스 또는 에셋 선택용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FName RequiredItemDataID;
};
