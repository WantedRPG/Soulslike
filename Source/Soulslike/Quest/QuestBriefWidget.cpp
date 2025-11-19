// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestBriefWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/DataTable.h"
#include "Quest/QuestData.h"

UQuestBriefWidget::UQuestBriefWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, QuestRowName(NAME_None)
{
	// 데이터 테이블 폴백 (기존 로직 유지)
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/Quest/DT_QuestData.DT_QuestData"));
	if (DataTableRef.Succeeded())
	{
		QuestDataTabel = DataTableRef.Object;
	}
}

void UQuestBriefWidget::InitializeFromRow(FName InRowName)
{
	QuestRowName = InRowName;
	// NativeConstruct에서 UI를 채움
}

void UQuestBriefWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuestName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlockQuestName")));
	QuestDescription = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlockQuestDescription")));
	OpenDetailButton = Cast<UButton>(GetWidgetFromName(TEXT("OpenDetailButton")));

	// 기존 데이터 채우기
	FName RowToUse = QuestRowName.IsNone() ? FName(TEXT("quest001")) : QuestRowName;

	if (QuestDataTabel)
	{
		const FQuestData* Row = QuestDataTabel->FindRow<FQuestData>(RowToUse, FString(TEXT("FindRow")));
		if (Row)
		{
			if (QuestName)
			{
				QuestName->SetText(Row->Text.Name);
			}
			if (QuestDescription)
			{
				QuestDescription->SetText(Row->Text.Description);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UQuestBriefWidget::NativeConstruct - Row not found: %s"), *RowToUse.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestBriefWidget::NativeConstruct - QuestDataTabel not set"));
	}

	// 버튼 바인딩
	if (OpenDetailButton)
	{
		OpenDetailButton->OnClicked.AddDynamic(this, &UQuestBriefWidget::HandleOpenDetailClicked);
	}
}

void UQuestBriefWidget::HandleOpenDetailClicked()
{
	// 현재 선택된 RowName을 전달
	if (!QuestRowName.IsNone())
	{
		OnOpenDetailRequested.Broadcast(QuestRowName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestBriefWidget::HandleOpenDetailClicked - QuestRowName is None"));
	}
}
