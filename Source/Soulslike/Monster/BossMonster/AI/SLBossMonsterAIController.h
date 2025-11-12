// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SLBossMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API ASLBossMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASLBossMonsterAIController();

protected:
	// BB, BT
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BossBehaviorTree;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBlackboardData> BossBlackboard;

	// Blackboard Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	// Behavior Tree Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	// BB에 Target을 저장할 때 사용. 이 때 Target은 플레이어
	void SetTargetActor(AActor* NewTarget);
	AActor* GetTargetActor() const;
};
