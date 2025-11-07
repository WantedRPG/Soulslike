// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    USLInventoryWidget(const FObjectInitializer& ObjectInitializer);
protected:
    virtual void NativeConstruct() override;
    //virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	void AddItemSlots(int32 Count);
protected:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* Grid_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setup")
	TSubclassOf<class USLItemSlot> ItemSlotWidgetClass;
};
