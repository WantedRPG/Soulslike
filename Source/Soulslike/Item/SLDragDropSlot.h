// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SLDragDropSlot.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLDragDropSlot : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	int PrevSlotIndex;
	UPROPERTY(VisibleAnywhere)
	class USLInventoryComponent* InventoryComponent;
};
