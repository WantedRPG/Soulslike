// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "없음"),
	Consumable UMETA(DisplayName = "소모템"),
	Throwable UMETA(DisplayName = "투척템"),
	Quest UMETA(DisplayName = "재료"),
	Material UMETA(DisplayName = "퀘스트"),
};

UENUM(BlueprintType)
enum class EItemActionType : uint8
{
	None UMETA(DisplayName = "없음"),
	Primary UMETA(DisplayName = "주 사용"),
	OnUnequip UMETA(DisplayName = "해제 시"),
	OnDrop UMETA(DisplayName = "버릴 시"),
	OnPickup UMETA(DisplayName = "획득 시"),
};