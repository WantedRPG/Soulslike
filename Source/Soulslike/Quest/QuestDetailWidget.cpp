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

UQuestDetailWidget::UQuestDetailWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)	
{
}

void UQuestDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Close 버튼 클릭 바인딩
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UQuestDetailWidget::HandleCloseButtonClicked);
	}

	// SetCurrentQuest 버튼 클릭 바인딩
	if (SetCurrentQuestButton)
	{
		SetCurrentQuestButton->OnClicked.AddDynamic(this, &UQuestDetailWidget::HandleSetCurrentQuestClicked);
	}
}

void UQuestDetailWidget::InitializeFromQuest(UQuest* InQuest)
{
	BoundQuest = InQuest;

	// Optional: 위젯의 텍스트를 여기서도 설정할 수 있음(호출부가 이미 설정하고 있으므로 중복 방지 가능)
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

	// QuestComponent 찾기: FirstPlayerController pawn 우선, 실패 시 OwningPlayerPawn
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

	// CurrentQuest 갱신
	QuestComp->CurrentQuest = BoundQuest->ID;
	UE_LOG(LogTemp, Log, TEXT("UQuestDetailWidget::HandleSetCurrentQuestClicked - CurrentQuest set to %s"), *BoundQuest->ID.ToString());

	// UI 갱신: QuestMenuWidget 인스턴스가 있으면 현재 진행 표시 위젯도 갱신
	if (QuestComp->QuestMenuWidget)
	{
		QuestComp->QuestMenuWidget->RefreshCurrentQuestProgression();
	}
}
