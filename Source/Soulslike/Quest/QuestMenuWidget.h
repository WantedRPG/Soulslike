// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestMenuWidget.generated.h"

class UUserWidget;
class UScrollBox;
class UQuestBriefWidget;

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

protected:
	virtual void NativeConstruct() override;

	// Data Table
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TObjectPtr<UDataTable> QuestDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Quest)
	TSubclassOf<UQuestBriefWidget> QuestBriefWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Quest)
	TObjectPtr<UUserWidget> QuestDetailWidget;
	
};
