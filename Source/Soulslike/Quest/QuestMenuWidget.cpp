// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestMenuWidget.h"
#include "Quest/QuestBriefWidget.h"
#include "Components/ScrollBox.h"
#include "Engine/DataTable.h"
#include "Quest.h"

UQuestMenuWidget::UQuestMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UQuestMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ActiveQuestScrollBox는 BindWidget으로 자동 연결되며, 없을 경우 GetWidgetFromName로 시도할 수 있습니다.
	if (!ActiveQuestScrollBox)
	{
		ActiveQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBoxActiveQuests")));
	}

	// FinishedQuestScrollBox도 시도 바인딩 (UMG에 "ScrollBoxFinishedQuests"로 추가해야 함)
	if (!FinishedQuestScrollBox)
	{
		FinishedQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBoxFinishedQuests")));
	}
}

// ActiveQuest ID 목록을 받아 스크롤박스를 초기화하고 QuestBriefWidget을 생성해 추가합니다.
void UQuestMenuWidget::InitializeFromQuestIDs(const TArray<FName>& InActiveQuestIDs)
{
	if (!ActiveQuestScrollBox)
	{
		ActiveQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBoxActiveQuests")));
	}

	if (!ActiveQuestScrollBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::InitializeFromQuestIDs - ActiveQuestScrollBox is not set"));
		return;
	}

	if (!QuestBriefWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::InitializeFromQuestIDs - QuestBriefWidget class not assigned"));
		return;
	}

	// 기존 항목 제거
	ActiveQuestScrollBox->ClearChildren();

	// 전달된 ID 목록으로 브리프 위젯 생성
	for (const FName& QuestID : InActiveQuestIDs)
	{
		UQuestBriefWidget* Brief = CreateWidget<UQuestBriefWidget>(GetWorld(), QuestBriefWidget);
		if (Brief)
		{
			Brief->InitializeFromRow(QuestID);
			ActiveQuestScrollBox->AddChild(Brief);
		}
	}
}

// ActiveQuest UQuest 인스턴스 목록 및 Finished quest ID 목록으로 스크롤박스를 초기화합니다.
void UQuestMenuWidget::RefreshQuestLists(const TArray<TObjectPtr<UQuest>>& InActiveQuests, const TArray<FName>& InFinishedQuestIDs)
{
	// Active 쪽 초기화
	if (!ActiveQuestScrollBox)
	{
		ActiveQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBoxActiveQuests")));
	}

	if (!ActiveQuestScrollBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::InitializeFromQuestLists - ActiveQuestScrollBox is not set"));
		// 계속 진행하되 Active 쪽은 건너뜀
	}
	else
	{
		if (!QuestBriefWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::InitializeFromQuestLists - QuestBriefWidget class not assigned"));
		}
		else
		{
			ActiveQuestScrollBox->ClearChildren();
			for (const TObjectPtr<UQuest>& QuestPtr : InActiveQuests)
			{
				if (!QuestPtr)
					continue;

				UQuestBriefWidget* Brief = CreateWidget<UQuestBriefWidget>(GetWorld(), QuestBriefWidget);
				if (Brief)
				{
					Brief->InitializeFromRow(QuestPtr->ID);
					ActiveQuestScrollBox->AddChild(Brief);
				}
			}
		}
	}

	// Finished 쪽 초기화
	if (!FinishedQuestScrollBox)
	{
		FinishedQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBoxFinishedQuests")));
	}

	if (!FinishedQuestScrollBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::InitializeFromQuestLists - FinishedQuestScrollBox is not set"));
		return;
	}

	if (!QuestBriefWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::InitializeFromQuestLists - QuestBriefWidget class not assigned"));
		return;
	}

	FinishedQuestScrollBox->ClearChildren();
	for (const FName& QuestID : InFinishedQuestIDs)
	{
		UQuestBriefWidget* Brief = CreateWidget<UQuestBriefWidget>(GetWorld(), QuestBriefWidget);
		if (Brief)
		{
			Brief->InitializeFromRow(QuestID);
			FinishedQuestScrollBox->AddChild(Brief);
		}
	}
}
