// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest.h"
#include "QuestComponent.generated.h"

class UQuest;
class UQuestDefinition;
class AQuestObjectiveActor;
class UQuestObjectiveDefinition;
class UQuestMenuWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	// Data Table
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TObjectPtr<UDataTable> QuestDataTable;

	// Quest list
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<FName> ActiveQuestsID;

	// Active quest which is shown on quest system ui
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	FName CurrentQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<FName> FinishedQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TArray<FName> FailedQuests;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TObjectPtr<class UQuestMenuWidget> QuestMenuWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Quest)
	TSubclassOf<class UQuestMenuWidget> QuestMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	TObjectPtr<class UQuestBriefWidget> QuestBriefWidget;

private:
	// 런타임에 생성된 UQuest 인스턴스들 (ActiveQuestsID -> UQuest 객체)
	UPROPERTY(Transient)
	TArray<TObjectPtr<UQuest>> ActiveQuestsInstance;

public:
	// UQuest에서 완료 알림을 받을 콜백
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void OnQuestCompleted(UQuest* CompletedQuest);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// ActiveQuestsID로부터 UQuest 인스턴스들을 생성하고, 필요하면 Objective 액터들을 스폰하여 연결합니다.
	void CreateActiveQuestInstances();

	// ActiveQuestsInstance 변경 시 UMG를 갱신
	void RefreshQuestMenu();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Start Quest, Track Objective Completion, Manage Quest stages, Award rewards, Trigger other game events
	void StartQuest(FName QuestID);
};
