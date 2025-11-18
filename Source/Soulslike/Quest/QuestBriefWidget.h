// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestBriefWidget.generated.h"

class UTextBlock;
class UDataTable;
class UButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOpenDetailRequested, FName);

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UQuestBriefWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UQuestBriefWidget(const FObjectInitializer& ObjectInitializer);

	// 행 식별자를 받아 위젯을 초기화합니다.
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void InitializeFromRow(FName InRowName);

	// 상세 보기 요청시 브로드캐스트되는 delegate
	FOnOpenDetailRequested OnOpenDetailRequested;

protected:
	virtual void NativeConstruct() override;

	// 버튼 클릭 핸들러
	UFUNCTION()
	void HandleOpenDetailClicked();

	// Data Table
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TObjectPtr<UDataTable> QuestDataTabel;

	// 선택한 데이터 테이블 행의 이름
	UPROPERTY()
	FName QuestRowName;

	UPROPERTY()
	TObjectPtr<class UTextBlock> QuestName;

	UPROPERTY()
	TObjectPtr<class UTextBlock> QuestDescription;

	// UMG에서 바인딩할 상세 열기 버튼 (위젯 이름: "OpenDetailButton")
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> OpenDetailButton;
};
