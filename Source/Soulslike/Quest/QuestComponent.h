// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest.h"
#include "QuestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	// Quest Properties

	// Quest list
	UPROPERTY(VisibleAnywhere, Category = Quest)
	TArray<TObjectPtr<UQuest>> ActiveQuests;

	// Active quest which is shown on quest system ui
	UPROPERTY(VisibleAnywhere, Category = Quest)
	TObjectPtr<UQuest> CurrentQuest;

	UPROPERTY(VisibleAnywhere, Category = Quest)
	TArray<TObjectPtr<UQuest>> FinishedQuests;

	UPROPERTY(VisibleAnywhere, Category = Quest)
	TArray<TObjectPtr<UQuest>> FailedQuests;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Start Quest, Track Objective Completion, Manage Quest stages, Award rewards, Trigger other game events
	void StartQuest(FName QuestID);

	// Load Quest.
};
