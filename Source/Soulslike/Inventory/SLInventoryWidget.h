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
	virtual void NativeOnInitialized() override;
    //virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

//인벤토리 드래그 기능 함수
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:

	void AddItemSlot(int32 InSlotIndex, FName InItemID, int32 InStackCount);
	void UpdateItemSlot(int32 InSlotIndex, FName InItemID, int32 InStackCount);
	void CleanInventory();
	void SetItemStackCount(int32 InSlotIndex,int32 InStackCount);
	void SetEmptySlot(int32 InSlotIndex);
	void SetRemoveSlot(int32 InSlotIndex);
	void SetInventoryComponent(class USLInventoryComponent* InInventoryCompoent);
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_All;
	
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* Grid_Item;
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_DragHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setup")
	TSubclassOf<class USLItemSlot> ItemSlotWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Row")
	int32 MaxRow = 8;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class USLInventoryComponent* InventoryComponent;

	bool bIsDragging = false;
	// 드래그 시작 시, 위젯의 좌상단 모서리와 마우스 커서 간의 오프셋
	FVector2D DragOffset;
};


