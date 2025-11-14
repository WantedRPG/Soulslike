// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest.h"
#include "QuestComponent.generated.h"

class UQuestDefinition;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	// Data Table
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TObjectPtr<UDataTable> QuestDataTabel;

	// Quest Properties<

	// Quest list
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<FName> ActiveQuests;

	// Active quest which is shown on quest system ui
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TObjectPtr<UQuest> CurrentQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<TObjectPtr<UQuest>> FinishedQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<TObjectPtr<UQuest>> FailedQuests;

	// UI
	// 인스턴스를 보관하는 포인터(런타임에 CreateWidget으로 생성됨)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	TObjectPtr<class UQuestMenuWidget> QuestMenuWidget;

	// 에디터에서 할당할 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Quest)
	TSubclassOf<class UQuestMenuWidget> QuestMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TObjectPtr<class UQuestBriefWidget> QuestBriefWidget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Start Quest, Track Objective Completion, Manage Quest stages, Award rewards, Trigger other game events
	void StartQuest(FName QuestID);
};
