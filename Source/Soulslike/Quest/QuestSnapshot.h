// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestSnapshot.generated.h"

// Quest.h에 선언된 EQuestState를 전방 선언
enum class EQuestState : uint8;

USTRUCT(BlueprintType)
struct FQuestObjectiveSnapshot
{
	GENERATED_BODY()

	// Objective 이름
	UPROPERTY(BlueprintReadOnly)
	FText Name;

	// Objective 완료 여부
	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;
};

USTRUCT(BlueprintType)
struct FQuestSnapshot
{
	GENERATED_BODY()

	// Quest ID
	UPROPERTY(BlueprintReadOnly)
	FName ID;

	// Quest title / name (Summary 사용)
	UPROPERTY(BlueprintReadOnly)
	FText Name;

	// 간단 요약
	UPROPERTY(BlueprintReadOnly)
	FText Summary;

	// 상세 설명
	UPROPERTY(BlueprintReadOnly)
	FText Description;

	// Objective 스냅샷 목록 (이름 + 완료 여부)
	UPROPERTY(BlueprintReadOnly)
	TArray<FQuestObjectiveSnapshot> Objectives;

	// 퀘스트 상태 (활성, 완료, 실패)
	UPROPERTY(BlueprintReadOnly)
	EQuestState State;

	// 완료 시각 (선택적)
	UPROPERTY(BlueprintReadOnly)
	FDateTime CompletedAt;
};