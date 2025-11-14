// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLAttackInfoStruct.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FSLAttackInfoStruct
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Knockback = 0.0f;
};
