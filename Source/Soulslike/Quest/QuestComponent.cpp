// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestComponent.h"
#include "QuestDefinition.h"
#include "Quest/QuestData.h"
#include "Blueprint/UserWidget.h"
#include "QuestBriefWidget.h"
#include "QuestMenuWidget.h"
#include "QuestObjectiveActor.h"
#include "QuestObjectiveDefinition.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Quest.h"
#include "TimerManager.h"
#include "Quest/QuestSnapshot.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 우선적으로 에디터에서 설정된 QuestDataTable을 사용합니다.
	const UDataTable* DataTable = QuestDataTable ? QuestDataTable.Get() : nullptr;

	// 에디터에서 할당되지 않았다면 기존의 경로로 폴백해서 로드합니다.
	if (!DataTable)
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/Quest/DT_QuestData.DT_QuestData"));
		if (DataTableRef.Succeeded())
		{
			DataTable = DataTableRef.Object;
			QuestDataTable = DataTableRef.Object;
		}
	}

	// ActiveQuestsID 초기화(여기서는 ID 목록만 채움; 인스턴스 생성은 BeginPlay에서 처리)
	if (DataTable)
	{
		TArray<FQuestData*> QuestDataRows;
		DataTable->GetAllRows<FQuestData>(TEXT("GetAllRows"), QuestDataRows);

		for (FQuestData* Row : QuestDataRows)
		{
			if (Row)
			{
				ActiveQuestsID.Add(Row->ID);
			}
		}
	}
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// QuestMenuWidget 생성 (실제 초기화는 CreateActiveQuestInstances 후 RefreshQuestMenu에서 수행)
	if (QuestMenuWidgetClass)
	{
		if (UWorld* World = GetWorld())
		{
			UQuestMenuWidget* Created = CreateWidget<UQuestMenuWidget>(World, QuestMenuWidgetClass);
			if (Created)
			{
				QuestMenuWidget = Created;
				QuestMenuWidget->AddToViewport();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestComponent::BeginPlay - QuestMenuWidgetClass not assigned. Please set it in the component defaults."));
	}

	// 임시 구현 : ActiveQuestsID의 첫 번째 퀘스트를 CurrentQuest로 설정
	if (ActiveQuestsID.Num() > 0)
	{
		CurrentQuest = ActiveQuestsID[0];
	}

	// ActiveQuestsID로부터 UQuest 인스턴스 생성 및 ObjectiveActor 스폰
	CreateActiveQuestInstances();
}

void UQuestComponent::CreateActiveQuestInstances()
{
	// Clear 기존 인스턴스
	for (TObjectPtr<UQuest>& Q : ActiveQuestsInstance)
	{
		if (Q)
		{
			Q->ConditionalBeginDestroy();
		}
	}
	ActiveQuestsInstance.Empty();

	const UDataTable* DataTable = QuestDataTable ? QuestDataTable.Get() : nullptr;
	if (!DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateActiveQuestInstances - QuestDataTable not set"));
		return;
	}

	UWorld* World = GetWorld();
	AActor* OwnerActor = GetOwner();

	for (const FName& QuestID : ActiveQuestsID)
	{
		// DataTable에서 행 찾기
		const FQuestData* Row = DataTable->FindRow<FQuestData>(QuestID, TEXT("CreateActiveQuestInstances"));
		if (!Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("CreateActiveQuestInstances - Row not found for ID: %s"), *QuestID.ToString());
			continue;
		}

		// UQuest 인스턴스 생성
		UQuest* NewQuest = NewObject<UQuest>(this);
		if (!NewQuest)
			continue;

		NewQuest->ID = Row->ID;
		NewQuest->Summary = Row->Text.Name;
		NewQuest->Description = Row->Text.Description;

		// QuestDataAsset(QuestDefinition)에서 ObjectiveDefinitions를 읽어 액터 스폰을 UQuest에 위임
		UQuestDefinition* QuestDef = Row->QuestDataAsset;
		if (QuestDef)
		{
			// UQuest에 스폰을 수행하도록 요청
			if (NewQuest->SpawnObjectiveActorsFromDefinition(QuestDef, OwnerActor))
			{
				// Quest 내에 저장된 QuestObjectiveActors는 UQuest가 관리함
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CreateActiveQuestInstances - QuestDef not set for quest %s"), *QuestID.ToString());
		}

		// 저장
		ActiveQuestsInstance.Add(NewQuest);
		UE_LOG(LogTemp, Log, TEXT("CreateActiveQuestInstances - Created UQuest instance for ID: %s"), *QuestID.ToString());
	}

	// 인스턴스 생성 후 UI 갱신
	RefreshQuestMenu();
}

void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQuestComponent::StartQuest(FName QuestID)
{
	// 전달 받은 퀘스트 ID를 현재 퀘스트로 등록
}

// Getter 구현: ActiveQuestsInstance 읽기 전용으로 반환
const TArray<TObjectPtr<UQuest>>& UQuestComponent::GetActiveQuestsInstance() const
{
	return ActiveQuestsInstance;
}

void UQuestComponent::OnQuestCompleted(UQuest* CompletedQuest)
{
	if (!CompletedQuest)
		return;

	// ID 존재 여부 확인 및 ID 기반 처리 전에 스냅샷을 먼저 생성하고 저장합니다.
	// (Objective 액터 정리는 CleanupObjectives에서 파괴/비활성화를 수행하므로,
	// 스냅샷은 정리 이전에 만들어야 Objective 상태가 보존됩니다.)
	if (!CompletedQuest->ID.IsNone())
	{
		// ID 목록 업데이트: ActiveQuestsID에서 제거하고 FinishedQuests에 추가
		for (int32 i = ActiveQuestsID.Num() - 1; i >= 0; --i)
		{
			if (ActiveQuestsID[i] == CompletedQuest->ID)
			{
				ActiveQuestsID.RemoveAt(i);
				break;
			}
		}
		FinishedQuests.AddUnique(CompletedQuest->ID);

		// 스냅샷 생성 및 저장 (CleanupObjectives 이전)
		QuestSnapshots.Add(CompletedQuest->CreateSnapshot());

		// 완료된 퀘스트가 현재 표시 중인 퀘스트라면 CurrentQuest를 비운다.
		if (CurrentQuest == CompletedQuest->ID)
		{
			CurrentQuest = NAME_None;
		}
	}
	else
	{
		// ID가 없는 경우에도 안전하게 스냅샷을 생성할 수 있도록 처리할 수 있으나
		// 현재는 ID 기반 저장 로직과 함께 동작하도록 유지합니다.
	}

	// Quest 자신에게 Objective 정리 책임을 위임 (스냅샷 저장 후에 실행)
	CompletedQuest->CleanupObjectives();

	// ActiveQuestsInstance에서 제거
	for (int32 i = ActiveQuestsInstance.Num() - 1; i >= 0; --i)
	{
		if (ActiveQuestsInstance[i].Get() == CompletedQuest)
		{
			ActiveQuestsInstance.RemoveAt(i);
			break;
		}
	}

	// UI 갱신: 일정 시간 대기한 뒤 갱신하여 UCurrentQuestProgressionWidget이 완료 후에도 남도록 함.
	const float DelayBeforeHidingCurrentQuestWidget = 2.0f;
	if (UWorld* World = GetWorld())
	{
		FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &UQuestComponent::RefreshQuestMenu);
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(Handle, Del, DelayBeforeHidingCurrentQuestWidget, false);
	}
	else
	{
		// World가 없으면 즉시 갱신
		RefreshQuestMenu();
	}

	UE_LOG(LogTemp, Log, TEXT("UQuestComponent::OnQuestCompleted - Quest %s completed."), *CompletedQuest->ID.ToString());
}

void UQuestComponent::RefreshQuestMenu()
{
	if (QuestMenuWidget)
	{
		// Active UQuest 인스턴스와 Finished ID 목록을 함께 전달
		QuestMenuWidget->RefreshQuestLists(ActiveQuestsInstance, FinishedQuests);
		QuestMenuWidget->RefreshCurrentQuestProgression();
	}
}
