// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Attribute/SLMonsterAttributeSet.h"
#include "SLMonsterbase.generated.h"

UCLASS()
class SOULSLIKE_API ASLMonsterbase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	ASLMonsterbase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class USLMonsterAttributeSet> MonsterAttributes;

public:	
	virtual void BeginPlay() override;
};
