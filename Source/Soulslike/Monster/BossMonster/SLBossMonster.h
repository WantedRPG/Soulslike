// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Common/SLMonsterbase.h"
#include "Abilities/GameplayAbility.h"
#include "Data/SLMontageData.h"
#include "Data/SLMonsterAttack.h"
#include "SLBossMonster.generated.h"

/**
 * 각각의 보스 몬스터 블루프린트클래스가 공통으로 상속하는 클래스
 * 게임플레이어빌리티를 부여하는 기능을 함
 */

UCLASS()
class SOULSLIKE_API ASLBossMonster : public ASLMonsterbase
{
	GENERATED_BODY()
	
public:
	ASLBossMonster();

public:
    UFUNCTION(BlueprintCallable)
    void DropItem();
    virtual void BeginPlay() override;

protected:
    // 몽타주 데이터에셋
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<USLMontageData> MontageDataPDA;

    // 공격 게임어빌리티 데이터에셋
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<USLMonsterAttack> AttackPDA;

    // BPGA_Turn 블루프린트 클래스를 보관할 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class UGameplayAbility> TurnAbilityClass;

    // BPGA_Move 블루프린트 클래스를 보관할 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class UGameplayAbility> MoveAbilityClass;

    // BPGA_Dead 블루프린트 클래스를 보관할 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class UGameplayAbility> DeadAbilityClass;

    // BPGA_Grogy 블루프린트 클래스를 보관할 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class UGameplayAbility> GrogyAbilityClass;

    // BPGA_Intro 블루프린트 클래스를 보관할 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class UGameplayAbility> IntroAbilityClass;

    // 드롭 아이템 액터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class ASLItemPickupActor> ItemPickupActorClass;
};
