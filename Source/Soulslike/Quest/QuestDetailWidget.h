// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDetailWidget.generated.h"

class UScrollBox;
class UTextBlock;

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
};
