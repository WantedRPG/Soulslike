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
	virtual void NativeDestruct() override;
public:

	void AddItemSlot(int32 InSlotIndex, FName InItemID, int32 InStackCount);
	void UpdateItemSlot(int32 InSlotIndex, FName InItemID, int32 InStackCount);
	void ShowGetItemUI();
	void HiddenGetItemUI();
	void CleanInventory();
	void SetItemStackCount(int32 InSlotIndex,int32 InStackCount);
	void SetEmptySlot(int32 InSlotIndex);
	void SetRemoveSlot(int32 InSlotIndex);

protected:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* Grid_Item;
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_GetItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setup")
	TSubclassOf<class USLItemSlot> ItemSlotWidgetClass;
	int32 MaxRow = 8;
	
};


