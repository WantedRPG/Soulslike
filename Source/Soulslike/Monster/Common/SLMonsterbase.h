// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet/SLAttributeSet.h"
#include "SLSignalComponent.h"
#include "Monster/Common/SLAttackInfoStruct.h"
#include "SLMonsterbase.generated.h"

UCLASS()
class SOULSLIKE_API ASLMonsterbase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	ASLMonsterbase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class USLAttributeSet> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USLSignalComponent> SignalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAcess = "true"))
	FSLAttackInfoStruct CurrentAttackInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> DamageGEClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> InitGEClass;

public:	
	virtual void BeginPlay() override;

	// 플레이어 공격 게임이펙트 적용
	UFUNCTION(BlueprintCallable)
	void ApplyHitToTarget(AActor* HitActor);

	// 몬스터 죽음
	UFUNCTION(BlueprintCallable)
	void MonsterDead();

	// 몬스터 그로기(상태이상)
	UFUNCTION(BlueprintCallable)
	void MonsterGrogy();
};
