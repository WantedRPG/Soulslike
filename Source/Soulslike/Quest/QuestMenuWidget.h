// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestMenuWidget.generated.h"

class UUserWidget;
class UScrollBox;
class UQuestBriefWidget;
class UMenuAnchor;
class UQuest;

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UQuestMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UQuestMenuWidget(const FObjectInitializer& ObjectInitializer);

	// UMG Designer에서 추가한 ScrollBox를 자동으로 바인딩합니다.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ActiveQuestScrollBox;

	// 완료된 퀘스트용 ScrollBox (UMG에 추가해주어야 합니다: "ScrollBoxFinishedQuests")
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> FinishedQuestScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuAnchor> QuestDetailMenuAnchor;

	// ActiveQuest ID 목록으로 위젯을 초기화합니다.
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void InitializeFromQuestIDs(const TArray<FName>& InActiveQuestIDs);

	// ActiveQuest UQuest 인스턴스 목록 및 FinishedQuest ID 목록을 받아 UI를 초기화합니다.
	// Active 목록은 UQuest 인스턴스 배열을 사용하고, Finished 목록은 ID(FName) 배열을 사용합니다.
	void RefreshQuestLists(const TArray<TObjectPtr<UQuest>>& InActiveQuests, const TArray<FName>& InFinishedQuestIDs);

	// 상세 위젯 클래스 (UMG에서 설정)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Quest)
	TSubclassOf<UUserWidget> QuestDetailWidgetClass;

protected:
	virtual void NativeConstruct() override;

	// Data Table (필요 시 백업/폴백 용도)
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TObjectPtr<UDataTable> QuestDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Quest)
	TSubclassOf<UQuestBriefWidget> QuestBriefWidget;

	// 현재 표시 중인 상세 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<UUserWidget> QuestDetailWidgetInstance;

	// 상세 표시 요청 처리
	UFUNCTION()
	void ShowQuestDetail(FName QuestID);
	
};
