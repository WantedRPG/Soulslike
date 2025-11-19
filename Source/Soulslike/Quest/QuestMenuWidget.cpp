// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestMenuWidget.h"
#include "Quest/QuestBriefWidget.h"
#include "Quest/QuestDetailWidget.h"
#include "Quest/QuestComponent.h"
#include "Quest/CurrentQuestProgressionWidget.h"
#include "Components/ScrollBox.h"
#include "Components/MenuAnchor.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "Quest.h"
#include "QuestObjectiveActor.h"

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
			// 상세 요청 바인딩 (이 함수 모드에서는 추가 바인딩 필요 시 사용)
			Brief->OnOpenDetailRequested.AddUObject(this, &UQuestMenuWidget::ShowQuestDetail);
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
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::RefreshQuestLists - ActiveQuestScrollBox is not set"));
		// 계속 진행하되 Active 쪽은 건너뜀
	}
	else
	{
		if (!QuestBriefWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::RefreshQuestLists - QuestBriefWidget class not assigned"));
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
					// 상세 열기 요청 바인딩
					Brief->OnOpenDetailRequested.AddUObject(this, &UQuestMenuWidget::ShowQuestDetail);
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
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::RefreshQuestLists - FinishedQuestScrollBox is not set"));
		return;
	}

	if (!QuestBriefWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::RefreshQuestLists - QuestBriefWidget class not assigned"));
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
			// 완료된 항목에서도 상세 열기 허용
			Brief->OnOpenDetailRequested.AddUObject(this, &UQuestMenuWidget::ShowQuestDetail);
		}
	}
}

void UQuestMenuWidget::ShowQuestDetail(FName QuestID)
{
	// 기존 인스턴스 제거
	if (QuestDetailWidgetInstance)
	{
		QuestDetailWidgetInstance->RemoveFromParent();
		QuestDetailWidgetInstance = nullptr;
	}

	// QuestComponent에서 ActiveQuestsInstance를 찾아 QuestID로 검색
	UQuestComponent* QuestComp = nullptr;
	// 우선 소유 플레이어의 pawn에서 찾기
	if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			QuestComp = Pawn->FindComponentByClass<UQuestComponent>();
		}
	}

	// 실패 시 GetOwningPlayerPawn로 재시도
	if (!QuestComp)
	{
		if (APawn* OwningPawn = GetOwningPlayerPawn())
		{
			QuestComp = OwningPawn->FindComponentByClass<UQuestComponent>();
		}
	}

	if (!QuestComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::ShowQuestDetail - QuestComponent not found"));
		return;
	}

	const TArray<TObjectPtr<UQuest>>& ActiveList = QuestComp->GetActiveQuestsInstance();
	UQuest* FoundQuest = nullptr;
	for (const TObjectPtr<UQuest>& Q : ActiveList)
	{
		if (Q && Q->ID == QuestID)
		{
			FoundQuest = Q.Get();
			break;
		}
	}

	if (!QuestDetailWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::ShowQuestDetail - QuestDetailWidgetClass not assigned"));
		return;
	}

	// 상세 위젯 생성
	UQuestDetailWidget* Detail = CreateWidget<UQuestDetailWidget>(GetWorld(), QuestDetailWidgetClass);
	Detail->InitializeFromQuest(FoundQuest);
	if (!Detail)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::ShowQuestDetail - Failed to create QuestDetailWidget"));
		return;
	}

	QuestDetailWidgetInstance = Detail;
	if (QuestDetailMenuAnchor)
	{
		QuestDetailMenuAnchor->AddChild(Detail);
	}
	Detail->AddToViewport();

	// 찾은 퀘스트 인스턴스로 상세 정보 설정
	if (FoundQuest)
	{
		if (Detail->QuestNameTextBlock)
		{
			Detail->QuestNameTextBlock->SetText(FoundQuest->Summary);
		}
		if (Detail->QuestDescTextBlock)
		{
			Detail->QuestDescTextBlock->SetText(FoundQuest->Description);
		}

		// QuestObjectiveScrollBox가 있고, QuestObjectiveActors가 있으면 간단히 목록을 추가할 수 있음.
		// (구체적인 Objective 위젯이 없으므로 텍스트로만 추가하는 예시)
		if (Detail->QuestObjectiveScrollBox)
		{
			Detail->QuestObjectiveScrollBox->ClearChildren();
			for (const TObjectPtr<AQuestObjectiveActor>& ObjActorPtr : FoundQuest->QuestObjectiveActors)
			{
				if (!ObjActorPtr)
					continue;

				// 간단한 텍스트 위젯 생성으로 Objective 표시 (UMG에서 적절한 위젯을 사용하는 것이 권장됨)
				UTextBlock* TB = NewObject<UTextBlock>(Detail);
				if (TB)
				{
					FText ObjText = ObjActorPtr->GetObjectiveName();
					TB->SetText(ObjText);
					Detail->QuestObjectiveScrollBox->AddChild(TB);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::ShowQuestDetail - Quest %s not found in ActiveQuestsInstance"), *QuestID.ToString());
	}
}

void UQuestMenuWidget::RefreshCurrentQuestProgression()
{
	// 기존 인스턴스 제거 (먼저 Unbind 해주기)
	if (CurrentQuestProgressionWidgetInstance)
	{
		if (UCurrentQuestProgressionWidget* Prev = Cast<UCurrentQuestProgressionWidget>(CurrentQuestProgressionWidgetInstance.Get()))
		{
			Prev->UnbindFromQuest();
		}
		CurrentQuestProgressionWidgetInstance->RemoveFromParent();
		CurrentQuestProgressionWidgetInstance = nullptr;
	}

	if (!CurrentQuestProgressionMenuAnchor)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UQuestMenuWidget::RefreshCurrentQuestProgression - Anchor not set"));
		return;
	}

	if (!CurrentQuestProgressionWidgetClass)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UQuestMenuWidget::RefreshCurrentQuestProgression - WidgetClass not set"));
		return;
	}

	// QuestComponent 검색 (FirstPlayerController pawn 우선, 실패 시 OwningPlayerPawn)
	UQuestComponent* QuestComp = nullptr;
	if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			QuestComp = Pawn->FindComponentByClass<UQuestComponent>();
		}
	}
	if (!QuestComp)
	{
		if (APawn* OwningPawn = GetOwningPlayerPawn())
		{
			QuestComp = OwningPawn->FindComponentByClass<UQuestComponent>();
		}
	}

	if (!QuestComp)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UQuestMenuWidget::RefreshCurrentQuestProgression - QuestComponent not found"));
		return;
	}

	// CurrentQuest가 비어있으면 새로 생성하지 않고 바로 반환 (이미 이전 인스턴스는 제거됨).
	if (QuestComp->CurrentQuest.IsNone())
	{
		UE_LOG(LogTemp, Verbose, TEXT("UQuestMenuWidget::RefreshCurrentQuestProgression - No current quest, not creating progression widget"));
		return;
	}

	// CreateWidget: OwningPlayer 우선
	APlayerController* OwningPC = GetOwningPlayer();
	UCurrentQuestProgressionWidget* ProgressWidget = nullptr;
	if (OwningPC)
	{
		ProgressWidget = CreateWidget<UCurrentQuestProgressionWidget>(OwningPC, CurrentQuestProgressionWidgetClass);
	}
	else
	{
		ProgressWidget = CreateWidget<UCurrentQuestProgressionWidget>(GetWorld(), CurrentQuestProgressionWidgetClass);
	}

	if (!ProgressWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::RefreshCurrentQuestProgression - Failed to create ProgressWidget"));
		return;
	}

	CurrentQuestProgressionWidgetInstance = ProgressWidget;
	CurrentQuestProgressionMenuAnchor->AddChild(ProgressWidget);

	// CurrentQuest로 UQuest 인스턴스 검색 (컴포넌트 내부 Active 인스턴스 사용)
	const TArray<TObjectPtr<UQuest>>& ActiveList = QuestComp->GetActiveQuestsInstance();
	UQuest* FoundQuest = nullptr;
	for (const TObjectPtr<UQuest>& Q : ActiveList)
	{
		if (Q && Q->ID == QuestComp->CurrentQuest)
		{
			FoundQuest = Q.Get();
			break;
		}
	}

	if (!FoundQuest)
	{
		UE_LOG(LogTemp, Verbose, TEXT("UQuestMenuWidget::RefreshCurrentQuestProgression - CurrentQuest not found in ActiveQuestsInstance"));
		// CurrentQuest가 유효하지 않다면 방금 생성한 위젯은 제거
		if (CurrentQuestProgressionWidgetInstance)
		{
			if (UCurrentQuestProgressionWidget* Prev = Cast<UCurrentQuestProgressionWidget>(CurrentQuestProgressionWidgetInstance.Get()))
			{
				Prev->UnbindFromQuest();
			}
			CurrentQuestProgressionWidgetInstance->RemoveFromParent();
			CurrentQuestProgressionWidgetInstance = nullptr;
		}
		return;
	}

	// ProgressWidget에 텍스트 적용
	if (ProgressWidget->QuestTextBlock)
	{
		ProgressWidget->QuestTextBlock->SetText(FoundQuest->Summary);
	}

	if (ProgressWidget->ObjectiveTextBlock)
	{
		FText ObjText = FText::GetEmpty();
		if (FoundQuest->QuestObjectiveActors.Num() > 0 && FoundQuest->QuestObjectiveActors[0] != nullptr)
		{
			ObjText = FoundQuest->QuestObjectiveActors[0]->GetObjectiveName();
		}
		ProgressWidget->ObjectiveTextBlock->SetText(ObjText);
	}

	// 새로 생성한 ProgressWidget에 바인딩
	ProgressWidget->BindToQuest(FoundQuest);
}
