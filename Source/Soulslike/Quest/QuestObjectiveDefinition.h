// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestObjectiveDefinition.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UQuestObjectiveDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 스폰 위치(레벨 내 Transform). 에디터/블루프린트에서 변경 가능.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FTransform SpawnTransform;
};
