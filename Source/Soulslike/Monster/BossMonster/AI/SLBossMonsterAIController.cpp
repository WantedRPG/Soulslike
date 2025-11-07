#include "SLBossMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Monster/BossMonster/SLBossMonster.h"

ASLBossMonsterAIController::ASLBossMonsterAIController()
{
	// 생성자에서 기본 컴포넌트 초기화
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/Monster/BossMonster/AI/BT_BossMonster.BT_BossMonster"));
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Game/Monster/BossMonster/AI/BB_BossMonster.BB_BossMonster"));

	if (BT.Succeeded())  BossBehaviorTree = BT.Object;
	if (BB.Succeeded())  BossBlackboard = BB.Object;
}

void ASLBossMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ASLBossMonster* Boss = Cast<ASLBossMonster>(InPawn))
	{
		if (BossBehaviorTree)
		{
			// Blackboard 초기화 후 BT 실행
			if (BossBehaviorTree->BlackboardAsset)
			{
				BlackboardComp->InitializeBlackboard(*BossBehaviorTree->BlackboardAsset);
			}

			RunBehaviorTree(BossBehaviorTree);
		}
	}
}

void ASLBossMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
	if (BehaviorComp)
	{
		BehaviorComp->StopTree();
	}
}

void ASLBossMonsterAIController::SetTargetActor(AActor* NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), NewTarget);
	}
}

AActor* ASLBossMonsterAIController::GetTargetActor() const
{
	if (BlackboardComp)
	{
		return Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	}
	return nullptr;
}
