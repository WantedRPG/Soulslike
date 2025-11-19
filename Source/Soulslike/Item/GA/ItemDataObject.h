// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataObject.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UItemDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	FName ItemID;
};
