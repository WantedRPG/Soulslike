// Fill out your copyright notice in the Description page of Project Settings.


#include "SLBossMonster.h"
#include "Abilities/GameplayAbility.h"
#include "AI/SLBossMonsterAIController.h"
#include <Item/SLItemManagerSubsystem.h>
#include <Item/SLItemData.h>
#include "Item/SLItemPickupActor.h"
#include <Kismet/GameplayStatics.h>

ASLBossMonster::ASLBossMonster()
{
    // 보스가 스폰/배치되면 자동으로 보스 AI가 장착되도록
    AIControllerClass = ASLBossMonsterAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    static ConstructorHelpers::FClassFinder<ASLItemPickupActor> ItemActorFinder(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Item/BP_PickupItem.BP_PickupItem_C'"));
    if (ItemActorFinder.Succeeded())
    {
        ItemPickupActorClass = ItemActorFinder.Class;
    }
}

void ASLBossMonster::DropItem()
{
    int32 RandomNumber = FMath::RandRange(1, 10);

    //숫자를 FName 형식에 맞게 문자열로 포맷
    FString NumberString = FString::Printf(TEXT("%03d"), RandomNumber);
    FName RandomKey = FName(*("Item_" + NumberString));

    if (USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance()))
    {
        if (USLItemData* ItemData = ItemManager->GetItemData(RandomKey))
        {

            if (ASLItemPickupActor* DropItem = GetWorld()->SpawnActorDeferred<ASLItemPickupActor>(ItemPickupActorClass, GetActorTransform()))
            {
                DropItem->SetItemData(ItemData->ItemID, 1);
                UGameplayStatics::FinishSpawningActor(DropItem, GetActorTransform());
            }
        }
    }
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

    // Give Move GA 
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
    // Give Dead GA 
    if (DeadAbilityClass)
    {
        FGameplayAbilitySpec Spec(DeadAbilityClass, 1, 0, MontageDataPDA);
        ASC->GiveAbility(Spec);
        UE_LOG(LogTemp, Log, TEXT("%s: GA_Dead granted"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: DeadAbilityClass is null"), *GetName());
    }
    // Give Grogy GA 
    if (GrogyAbilityClass)
    {
        FGameplayAbilitySpec Spec(GrogyAbilityClass, 1, 0, MontageDataPDA);
        ASC->GiveAbility(Spec);
        UE_LOG(LogTemp, Log, TEXT("%s: GA_Grogy granted"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: GrogyAbilityClass is null"), *GetName());
    }
    // Give Intro GA 
    if (IntroAbilityClass)
    {
        FGameplayAbilitySpec Spec(IntroAbilityClass, 1, 0, MontageDataPDA);
        ASC->GiveAbility(Spec);
        UE_LOG(LogTemp, Log, TEXT("%s: GA_Intro granted"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: IntroAbilityClass is null"), *GetName());
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
