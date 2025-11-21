// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.generated.h"

class AQuestObjectiveActor;
class UQuestDefinition;
struct FQuestSnapshot;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestProgressUpdated, UQuest*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestCompletedDelegate, UQuest*);

UENUM()
enum class EQuestState : uint8
{
	NotActivated UMETA(DisplayName = "NotActivatedQuest"),
	Active UMETA(DisplayName = "ActiveQuest"), // InProgress
	Completed UMETA(DisplayName = "CompletedQuest"),
	Failed UMETA(DisplayName = "FailedQuest")
};

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

	// Objectives of a quest (스폰된 액터들을 보관)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<TObjectPtr<AQuestObjectiveActor>> QuestObjectiveActors;

	// Broadcast when quest progress changes (objective progressed, etc.)
	FOnQuestProgressUpdated OnQuestProgressUpdated;

	// Broadcast when quest is completed
	FOnQuestCompletedDelegate OnQuestCompletedDelegate;

private:
	EQuestState State = EQuestState::NotActivated;

	int32 CurrentObjectiveIndex = 0;

public:
	UFUNCTION()
	void ActivateQuest();

	UFUNCTION()
	void ProgressQuest();

	// 퀘스트 완료 처리: 소유 컴포넌트에 알리기 위해 호출
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CompleteQuest();

	UFUNCTION(Category = "Quest")
	FORCEINLINE int32 GetCurrentObjectiveIndex() { return CurrentObjectiveIndex; }

	// 현재 인덱스에 해당하는 Objective의 TriggerBox만 활성화하고 나머지는 비활성화
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UpdateObjectiveActors();

	// QuestDefinition 기반으로 Objective 액터들을 스폰해서 QuestObjectiveActors에 저장합니다.
	// OwnerActor가 null이면 World를 찾을 수 없으므로 실패합니다.
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool SpawnObjectiveActorsFromDefinition(UQuestDefinition* QuestDef, AActor* OwnerActor);

	// Quest가 보유한 Objective 액터들을 정리(비활성화 + 지연 파괴)
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CleanupObjectives();

	// 퀘스트 상태로부터 완료 스냅샷 생성
	FQuestSnapshot CreateSnapshot() const;
};