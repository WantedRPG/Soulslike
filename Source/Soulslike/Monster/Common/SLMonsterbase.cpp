// Fill out your copyright notice in the Description page of Project Settings.


#include "SLMonsterbase.h"
#include "AbilitySystemComponent.h"

// Sets default values
ASLMonsterbase::ASLMonsterbase()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
    MonsterAttributes = CreateDefaultSubobject<USLMonsterAttributeSet>(TEXT("MonsterAttributes"));
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
        if (MonsterAttributes)
        {
            UE_LOG(LogTemp, Log, TEXT("%s AttributeSet 초기화 완료"), *GetName());
        }
    }

}

