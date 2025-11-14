// Fill out your copyright notice in the Description page of Project Settings.


#include "SLMonsterbase.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"

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


