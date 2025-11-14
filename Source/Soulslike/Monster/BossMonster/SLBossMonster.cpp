// Fill out your copyright notice in the Description page of Project Settings.


#include "SLBossMonster.h"
#include "Abilities/GameplayAbility.h"
#include "AI/SLBossMonsterAIController.h"

ASLBossMonster::ASLBossMonster()
{
    // 보스가 스폰/배치되면 자동으로 보스 AI가 장착되도록
    AIControllerClass = ASLBossMonsterAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ASLBossMonster::BeginPlay()
{
    Super::BeginPlay();

    // 보스용 데이터에셋 누락 시 스킵(로그)
    if (!MontageDataPDA || !AttackPDA)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: MontageDataPDA is not assigned"), *GetName());
        return;
    }

    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: ASC not found"), *GetName());
        return;
    }

    // Give Move Ga 
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
    // Give Turn GA
    if (TurnAbilityClass)
    {
        FGameplayAbilitySpec Spec(TurnAbilityClass, 1, 0, MontageDataPDA);
        ASC->GiveAbility(Spec);
        UE_LOG(LogTemp, Log, TEXT("%s: GA_Turn granted"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: TurnAbilityClass is null"), *GetName());
    }

    // Give Normal Attack GA
    if (ASC && AttackPDA)
    {
        for (const auto& AbilityClass : AttackPDA->NormalAttacks)
        {
            if (AbilityClass)
            {
                ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0, this));
            }
        }
    }

    // Give Special Attack GA
    if (ASC && AttackPDA)
    {
        for (const auto& AbilityClass : AttackPDA->SpecialAttacks)
        {
            if (AbilityClass)
            {
                ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0, this));
            }
        }
    }

}
