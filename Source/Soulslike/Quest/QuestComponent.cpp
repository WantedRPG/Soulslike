// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestComponent.h"
#include "QuestDefinition.h"
#include "Quest/QuestData.h"
#include "Blueprint/UserWidget.h"
#include "QuestBriefWidget.h"
#include "QuestMenuWidget.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/Quest/DT_QuestData.DT_QuestData"));

	if (DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		int32 NumRow = DataTable->GetRowMap().Num();

		TArray<FQuestData*> QuestDataRows;

		DataTable->GetAllRows<FQuestData>(TEXT("GetAllRows"), QuestDataRows);

		for (int Index = 0; Index < NumRow; Index++)
		{
			ActiveQuests.Add(QuestDataRows[Index]->ID);
		}	
	}

	// 생성자에서는 UMG 인스턴스를 만들지 않습니다. BeginPlay에서 CreateWidget으로 생성합니다.
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// QuestMenuWidgetClass가 에디터에서 할당되어 있으면 런타임에 CreateWidget으로 인스턴스 생성
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
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UQuestComponent::BeginPlay - Failed to create QuestMenuWidget instance"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestComponent::BeginPlay - QuestMenuWidgetClass not assigned. Please set it in the component defaults."));
	}

	// 기존에 있던 로직(필요 시 QuestBriefWidgetArray 사용 등) 여기에 추가하세요.
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::StartQuest(FName QuestID)
{
	// 전달 받은 퀘스트 ID를 현재 퀘스트로 등록

}
