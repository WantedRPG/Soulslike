// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestMenuWidget.h"
#include "Quest/QuestBriefWidget.h"
#include "Components/ScrollBox.h"
#include "Engine/DataTable.h"

UQuestMenuWidget::UQuestMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 기본 생성자 비워둠. 에디터에서 프로퍼티를 설정하세요.
}

void UQuestMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 에디터에서 바인드되지 않은 경우 위젯 트리에서 찾아보기 (선택적)
	if (!ActiveQuestScrollBox)
	{
		ActiveQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBoxActiveQuests")));
	}

	if (!ActiveQuestScrollBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::NativeConstruct - ActiveQuestScrollBox is not set"));
		return;
	}

	if (!QuestBriefWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::NativeConstruct - QuestBriefWidget class not assigned"));
		return;
	}

	// 데이터 테이블이 있으면 각 행마다 Brief 위젯을 생성해서 추가
	if (QuestDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestMenuWidget::NativeConstruct - QuestDataTable exist"));
		const TArray<FName> RowNames = QuestDataTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			UQuestBriefWidget* Brief = CreateWidget<UQuestBriefWidget>(GetWorld(), QuestBriefWidget);
			if (Brief)
			{
				// 생성 직후 해당 행으로 초기화
				Brief->InitializeFromRow(RowName);

				ActiveQuestScrollBox->AddChild(Brief);
			}
		}
	}
	else
	{
		// 데이터 테이블이 없는 경우 예시로 한 개 추가
		UQuestBriefWidget* Brief = CreateWidget<UQuestBriefWidget>(GetWorld(), QuestBriefWidget);
		if (Brief)
		{
			ActiveQuestScrollBox->AddChild(Brief);
		}
	}
}
