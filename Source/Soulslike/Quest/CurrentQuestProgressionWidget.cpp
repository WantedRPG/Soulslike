// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/CurrentQuestProgressionWidget.h"
#include "Components/TextBlock.h"
#include "Quest/Quest.h"
#include "QuestObjectiveActor.h"
#include "TimerManager.h"
#include "Components/MenuAnchor.h"

UCurrentQuestProgressionWidget::UCurrentQuestProgressionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCurrentQuestProgressionWidget::BindToQuest(UQuest* Quest)
{
	// 기존 바인드 해제
	UnbindFromQuest();

	if (!Quest)
		return;

	BoundQuest = Quest;

	// 초기 표시
	if (QuestTextBlock)
	{
		QuestTextBlock->SetText(Quest->Summary);
	}
	if (ObjectiveTextBlock)
	{
		FText ObjText = FText::GetEmpty();
		if (Quest->QuestObjectiveActors.Num() > 0 && Quest->QuestObjectiveActors[0] != nullptr)
		{
			ObjText = Quest->QuestObjectiveActors[0]->GetObjectiveName();
		}
		ObjectiveTextBlock->SetText(ObjText);
	}

	// 델리게이트 바인딩
	Quest->OnQuestProgressUpdated.AddUObject(this, &UCurrentQuestProgressionWidget::HandleQuestProgressUpdated);
	Quest->OnQuestCompletedDelegate.AddUObject(this, &UCurrentQuestProgressionWidget::HandleQuestCompleted);
}

void UCurrentQuestProgressionWidget::UnbindFromQuest()
{
	if (BoundQuest.IsValid())
	{
		if (UQuest* Q = BoundQuest.Get())
		{
			// non-dynamic multicast delegate 사용: 해당 오브젝트에 대한 모든 바인딩 제거
			Q->OnQuestProgressUpdated.RemoveAll(this);
			Q->OnQuestCompletedDelegate.RemoveAll(this);
		}
	}
	BoundQuest.Reset();
}

void UCurrentQuestProgressionWidget::HandleQuestProgressUpdated(UQuest* Quest)
{
	// ObjText 값 로그 출력
	UE_LOG(LogTemp, Log, TEXT("HandleQuestProgressUpdated"));

	if (!Quest || BoundQuest.Get() != Quest)
		return;

	// 진행 텍스트 갱신
	if (QuestTextBlock)
	{
		QuestTextBlock->SetText(Quest->Summary);
	}
	if (ObjectiveTextBlock)
	{
		FText ObjText = FText::GetEmpty();
		if (Quest->QuestObjectiveActors.Num() > 0 && Quest->QuestObjectiveActors[Quest->QuestObjectiveActors.Num() > 0 ? 0 : 0] != nullptr)
		{
			// 현재 간단히 첫 Objective 표시 (확장 가능)
			if (Quest->QuestObjectiveActors[Quest->GetCurrentObjectiveIndex()] != nullptr)
			{
				ObjText = Quest->QuestObjectiveActors[Quest->GetCurrentObjectiveIndex()]->GetObjectiveName();
			}
		}

		// ObjText 값 로그 출력
		UE_LOG(LogTemp, Log, TEXT("Current Objective Text: %s"), *ObjText.ToString());

		ObjectiveTextBlock->SetText(ObjText);
	}

	// "목표 완료" 메시지 생성 및 표시 후 1초 뒤 제거
	if (MessageMenuAnchor)
	{
		UTextBlock* Msg = NewObject<UTextBlock>(this);
		if (Msg)
		{
			Msg->SetText(FText::FromString(TEXT("Objective Complete!")));
			// 가운데 정렬
			Msg->SetJustification(ETextJustify::Center);
			MessageMenuAnchor->AddChild(Msg);

			FTimerHandle TimerHandle;
			FTimerDelegate RemoveDel = FTimerDelegate::CreateLambda([Msg]()
			{
				if (Msg && Msg->IsValidLowLevel())
				{
					Msg->RemoveFromParent();
				}
			});

			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().SetTimer(TimerHandle, RemoveDel, 2.0f, false);
			}
		}
	}
}

void UCurrentQuestProgressionWidget::HandleQuestCompleted(UQuest* Quest)
{
	// ObjText 값 로그 출력
	UE_LOG(LogTemp, Log, TEXT("HandleQuestProgressUpdated"));

	if (!Quest || BoundQuest.Get() != Quest)
		return;

	// 퀘스트 완료 시 표시 갱신(예: 완료 텍스트) 및 자동 언바인드
	if (QuestTextBlock)
	{
		QuestTextBlock->SetText(FText::FromString(TEXT("Completed: ") + Quest->Summary.ToString()));
	}
	if (ObjectiveTextBlock)
	{
		ObjectiveTextBlock->SetText(FText::GetEmpty());
	}

	// "퀘스트 완료" 메시지 생성 및 표시 후 1초 뒤 제거
	if (MessageMenuAnchor)
	{
		UTextBlock* Msg = NewObject<UTextBlock>(this);
		if (Msg)
		{
			Msg->SetText(FText::FromString(TEXT("Quest Complete!")));
			// 가운데 정렬
			Msg->SetJustification(ETextJustify::Center);
			MessageMenuAnchor->AddChild(Msg);

			FTimerHandle TimerHandle;
			FTimerDelegate RemoveDel = FTimerDelegate::CreateLambda([Msg]()
			{
				if (Msg && Msg->IsValidLowLevel())
				{
					Msg->RemoveFromParent();
				}
			});

			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().SetTimer(TimerHandle, RemoveDel, 2.0f, false);
			}
		}
	}

	// 완료 후 더 이상 바인드 유지할 필요 없음
	UnbindFromQuest();
}
