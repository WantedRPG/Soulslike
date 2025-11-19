// Fill out your copyright notice in the Description page of Project Settings.


#include "SLMonsterbase.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASLMonsterbase::ASLMonsterbase()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
    Attributes = CreateDefaultSubobject<USLAttributeSet>(TEXT("Attributes"));
    SignalComponent = CreateDefaultSubobject<USLSignalComponent>(TEXT("SignalComponent"));
}

UAbilitySystemComponent* ASLMonsterbase::GetAbilitySystemComponent() const
{
    return ASC;
}

void ASLMonsterbase::BeginPlay()
{
	Super::BeginPlay();

    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);

        // AttributeSet이 제대로 붙었는지 확인 로그
        if (Attributes)
        {
            UE_LOG(LogTemp, Log, TEXT("%s AttributeSet 초기화 완료"), *GetName());
        }

        // InitGEClass 배정 여부 확인
        if (!InitGEClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("InitGEClass is NOT assigned on %s"), *GetName());
            return;
        }

        // GE Context 만들기
        FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
        ContextHandle.AddSourceObject(this);

        // Spec 생성
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(InitGEClass, 1.f, ContextHandle);
        if (!SpecHandle.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to make GE Spec for %s"), *GetName());
            return;
        }

        // 초기 스탯 GE한 번 발동!
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        UE_LOG(LogTemp, Log, TEXT("Init Stat GE for %s"), *GetName());
    }

}

void ASLMonsterbase::ApplyHitToTarget(AActor* HitActor)
{
    if (!HitActor) return;

    // 대상 ASC 확인
    UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>();
    if (!TargetASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyHitToTarget: Target has no ASC"));
        return;
    }

    // 공격자 ASC 확인
    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyHitToTarget: Monster ASC is null"));
        return;
    }

    const float Damage = CurrentAttackInfo.Damage;
    const float Knockback = CurrentAttackInfo.Knockback;

    if (!DamageGEClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyHitToTarget: DamageGEClass is null"));
        return;
    }

    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    ContextHandle.AddSourceObject(this);

    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageGEClass, 1.0f, ContextHandle);
    if (!SpecHandle.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyHitToTarget: SpecHandle invalid"));
        return;
    }

    // Health를 깎으려면 음수로 전달
    SpecHandle.Data->SetSetByCallerMagnitude(
        FGameplayTag::RequestGameplayTag("Effect.Damage"),
        -Damage
    );

    TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

    // 넉백은 이벤트로 전달
    FGameplayEventData EventData;
    EventData.EventMagnitude = Knockback;
    EventData.Instigator = this;
    EventData.Target = HitActor;
    EventData.ContextHandle = ContextHandle;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        HitActor,
        FGameplayTag::RequestGameplayTag("Event.Hit"),
        EventData
    );

    UE_LOG(LogTemp, Log, TEXT("ApplyHitToTarget: Applied Damage=%.1f Knockback=%.1f"),
        Damage, Knockback);
}

void ASLMonsterbase::MonsterDead()
{
    UE_LOG(LogTemp, Log, TEXT("MonsterDead Called"));
    // AI 중지 (AIController와 BT 모두)
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        AICon->StopMovement();

        if (UBehaviorTreeComponent* BTComp = AICon->FindComponentByClass<UBehaviorTreeComponent>())
        {
            BTComp->StopTree(EBTStopMode::Safe);
        }
    }
    // 이동 정지
    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->DisableMovement();
        MoveComp->StopMovementImmediately();
    }

    // Ability 시스템에서 모든 Ability 정지, 죽음 어빌리티 실행
    if (ASC)
    {
        // 실행 중인 모든 어빌리티 중단
        ASC->CancelAllAbilities();

        FGameplayTagContainer DeadTags;
        DeadTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.Ability.Dead")));
        ASC->TryActivateAbilitiesByTag(DeadTags);
    }
}

void ASLMonsterbase::MonsterGrogy()
{
    // AI 중지 (AIController와 BT 모두)
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        AICon->StopMovement();

        if (UBrainComponent* Brain = AICon->GetBrainComponent())
        {
            Brain->PauseLogic(TEXT("Grogy"));
        }
    }
    // 이동 정지
    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->DisableMovement();
        MoveComp->StopMovementImmediately();
    }

    // Ability 시스템에서 모든 Ability 정지
    if (ASC)
    {
        // 실행 중인 모든 어빌리티 중단
        ASC->CancelAllAbilities();

        FGameplayTagContainer GrogyTags;
        GrogyTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.Ability.Grogy")));
        ASC->TryActivateAbilitiesByTag(GrogyTags);
    }
}


