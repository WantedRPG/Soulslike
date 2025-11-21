// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestDetailWidget.h"
#include "Components/Button.h"
#include "Quest/Quest.h"
#include "Quest/QuestComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "QuestMenuWidget.h"
#include "Quest/QuestSnapshot.h"
#include "Components/Widget.h"
#include "Components/ScrollBox.h"

UQuestDetailWidget::UQuestDetailWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)	
{
}

void UQuestDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Close 버튼 연결
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UQuestDetailWidget::HandleCloseButtonClicked);
	}

	// SetCurrentQuest 버튼 연결
	if (SetCurrentQuestButton)
	{
		SetCurrentQuestButton->OnClicked.AddDynamic(this, &UQuestDetailWidget::HandleSetCurrentQuestClicked);
	}
}

void UQuestDetailWidget::InitializeFromQuest(UQuest* InQuest)
{
	BoundQuest = InQuest;

	// Optional: 퀘스트 인스턴스로부터 텍스트 채우기
	if (BoundQuest)
	{
		if (QuestNameTextBlock)
		{
			QuestNameTextBlock->SetText(BoundQuest->Summary);
		}
		if (QuestDescTextBlock)
		{
			QuestDescTextBlock->SetText(BoundQuest->Description);
		}
		// UQuest 인스턴스 기반일 때는 SetCurrentQuest 버튼 노출 (퀘스트 상태 접근자가 없으므로 기본 노출)
		if (SetCurrentQuestButton)
		{
			SetCurrentQuestButton->SetVisibility(ESlateVisibility::Visible);
			SetCurrentQuestButton->SetIsEnabled(true);
		}
	}
}

void UQuestDetailWidget::InitializeFromSnapshot(const FQuestSnapshot& InSnapshot)
{
	// 스냅샷으로 초기화할 때는 BoundQuest를 비움
	BoundQuest = nullptr;

	// 제목: 스냅샷의 Name 사용
	if (QuestNameTextBlock)
	{
		QuestNameTextBlock->SetText(InSnapshot.Name);
	}

	// 설명
	if (QuestDescTextBlock)
	{
		QuestDescTextBlock->SetText(InSnapshot.Description);
	}

	// Objective 목록 채우기 (간단 텍스트로, 완료 여부 표기)
	if (QuestObjectiveScrollBox)
	{
		QuestObjectiveScrollBox->ClearChildren();
		for (const FQuestObjectiveSnapshot& ObjSnap : InSnapshot.Objectives)
		{
			UTextBlock* TB = NewObject<UTextBlock>(this);
			if (TB)
			{
				FString Display = ObjSnap.Name.ToString();
				if (ObjSnap.bCompleted)
				{
					Display.Append(TEXT(" (Completed!)"));
				}
				TB->SetText(FText::FromString(Display));
				QuestObjectiveScrollBox->AddChild(TB);
			}
		}
	}

	// 스냅샷의 상태가 Completed면 SetCurrentQuest 버튼 숨김/비활성화
	if (SetCurrentQuestButton)
	{
		if (InSnapshot.State == EQuestState::Completed)
		{
			SetCurrentQuestButton->SetVisibility(ESlateVisibility::Collapsed);
			SetCurrentQuestButton->SetIsEnabled(false);
		}
		else
		{
			SetCurrentQuestButton->SetVisibility(ESlateVisibility::Visible);
			SetCurrentQuestButton->SetIsEnabled(true);
		}
	}
}

void UQuestDetailWidget::HandleCloseButtonClicked()
{
	RemoveFromParent();
}

void UQuestDetailWidget::HandleSetCurrentQuestClicked()
{
	if (!BoundQuest)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestDetailWidget::HandleSetCurrentQuestClicked - No bound quest"));
		return;
	}

	// QuestComponent 검색
	UQuestComponent* QuestComp = nullptr;

	// Try first player controller
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				QuestComp = Pawn->FindComponentByClass<UQuestComponent>();
			}
		}
	}

	// Fallback to owning player pawn
	if (!QuestComp)
	{
		if (APawn* OwningPawn = GetOwningPlayerPawn())
		{
			QuestComp = OwningPawn->FindComponentByClass<UQuestComponent>();
		}
	}

	if (!QuestComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestDetailWidget::HandleSetCurrentQuestClicked - QuestComponent not found"));
		return;
	}

	// CurrentQuest 설정
	QuestComp->CurrentQuest = BoundQuest->ID;
	UE_LOG(LogTemp, Log, TEXT("UQuestDetailWidget::HandleSetCurrentQuestClicked - CurrentQuest set to %s"), *BoundQuest->ID.ToString());

	// UI 갱신
	if (QuestComp->QuestMenuWidget)
	{
		QuestComp->QuestMenuWidget->RefreshCurrentQuestProgression();
	}
}
