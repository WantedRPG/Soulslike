// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDetailWidget.generated.h"

class UScrollBox;
class UTextBlock;
class UButton;
class UQuest;

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UQuestDetailWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UQuestDetailWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestDescTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> QuestObjectiveScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SetCurrentQuestButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	// 위젯에 표시할 퀘스트를 설정합니다.
	void InitializeFromQuest(UQuest* InQuest);

protected:
	// NativeConstruct에서 버튼 바인딩을 수행합니다.
	virtual void NativeConstruct() override;

	// Close 버튼 클릭 핸들러: 자기 자신을 제거/숨김 처리
	UFUNCTION()
	void HandleCloseButtonClicked();

	// SetCurrentQuest 버튼 클릭 핸들러: QuestComponent의 CurrentQuest를 갱신
	UFUNCTION()
	void HandleSetCurrentQuestClicked();

private:
	// 바인드된 퀘스트 포인터
	TObjectPtr<UQuest> BoundQuest;
};
