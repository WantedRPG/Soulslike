// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestBriefWidget.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "Quest/QuestData.h"

UQuestBriefWidget::UQuestBriefWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, QuestRowName(NAME_None)
{
	// 에디터에서 지정하지 않은 경우를 대비해 데이터테이블을 로드합니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/Quest/DT_QuestData.DT_QuestData"));
	if (DataTableRef.Succeeded())
	{
		QuestDataTabel = DataTableRef.Object;
	}
}

void UQuestBriefWidget::InitializeFromRow(FName InRowName)
{
	QuestRowName = InRowName;
	// 필요하면 여기서 즉시 UI 갱신 로직을 호출할 수 있으나,
	// NativeConstruct에서 UI 위젯 바인딩 후에 처리합니다.
}

void UQuestBriefWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuestName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlockQuestName")));
	QuestDescription = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlockQuestDescription")));

	// 저장된 행 이름이 없으면 기존 하드코드된 기본값을 사용
	FName RowToUse = QuestRowName.IsNone() ? FName(TEXT("quest001")) : QuestRowName;

	if (QuestDataTabel)
	{
		const FQuestData* Row = QuestDataTabel->FindRow<FQuestData>(RowToUse, FString(TEXT("FindRow")));
		if (Row)
		{
			UE_LOG(LogTemp, Log, TEXT("QuestName : %s"), *Row->Text.Name.ToString());

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
}
