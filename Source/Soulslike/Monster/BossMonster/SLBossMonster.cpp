// Fill out your copyright notice in the Description page of Project Settings.


#include "SLBossMonster.h"
#include "Abilities/GameplayAbility.h"
#include "AI/SLBossMonsterAIController.h"

ASLBossMonster::ASLBossMonster()
{
    // 보스가 스폰/배치되면 자동으로 보스 AI가 장착되도록
    AIControllerClass = ASLBossMonsterAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    // BPGA_Move 블루프린트 Ability 로드
    static ConstructorHelpers::FObjectFinder<UBlueprint> MoveGARef(
        TEXT("/Game/Monster/BossMonster/GA/BPGA_Move.BPGA_Move")
    );

    if (MoveGARef.Succeeded())
    {
        MoveAbilityClass = (UClass*)MoveGARef.Object->GeneratedClass;
    }

    // BPGA_NormalAttack 블루프린트 Ability 로드
    static ConstructorHelpers::FObjectFinder<UBlueprint> NormalAttackGARef(
        TEXT("/Game/Monster/BossMonster/GA/BPGA_NormalAttack.BPGA_NormalAttack")
    );

    if (NormalAttackGARef.Succeeded())
    {
        NormalAttackAbilityClass = (UClass*)NormalAttackGARef.Object->GeneratedClass;
    }
}

void ASLBossMonster::BeginPlay()
{
    Super::BeginPlay();

    // 보스용 데이터에셋 누락 시 스킵(로그)
    if (!MontageDataPDA)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: MontageDataPDA is not assigned"), *GetName());
        return;
    }

    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: ASC not found"), *GetName());
        return;
    }

    if (MoveAbilityClass)
    {
        FGameplayAbilitySpec Spec(MoveAbilityClass, 1, 0, MontageDataPDA);
        ASC->GiveAbility(Spec);
        UE_LOG(LogTemp, Log, TEXT("%s: GA_Move granted"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: MoveAbilityClass is null"), *GetName());
    }

    if (NormalAttackAbilityClass)
    {
        FGameplayAbilitySpec Spec(NormalAttackAbilityClass, 1, 0, MontageDataPDA);
        ASC->GiveAbility(Spec);
        UE_LOG(LogTemp, Log, TEXT("%s: GA_NormalAttack granted"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: NormalAttackAbilityClass is null"), *GetName());
    }
    
}
