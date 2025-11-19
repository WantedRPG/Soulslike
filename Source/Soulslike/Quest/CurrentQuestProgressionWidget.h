// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentQuestProgressionWidget.generated.h"

class UTextBlock;
class UQuest;
class UMenuAnchor;

UCLASS()
class SOULSLIKE_API UCurrentQuestProgressionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCurrentQuestProgressionWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ObjectiveTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuAnchor> MessageMenuAnchor;

	// 퀘스트에 바인딩 (진행/완료 콜백 등록) 및 초기 표시
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void BindToQuest(UQuest* Quest);

	// 바인딩 해제
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UnbindFromQuest();

protected:
	// 콜백 핸들러
	UFUNCTION()
	void HandleQuestProgressUpdated(UQuest* Quest);

	UFUNCTION()
	void HandleQuestCompleted(UQuest* Quest);

private:
	// 바운드된 퀘스트(약한 포인터)
	TWeakObjectPtr<UQuest> BoundQuest;
};
