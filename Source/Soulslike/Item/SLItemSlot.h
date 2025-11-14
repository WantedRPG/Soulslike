// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	USLItemSlot(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;

public:
	void SetItemSlotData(class USLItemData* NewItemData, int32 InStackCount, int32 InSlotIndex,class USLInventoryComponent* InInventoryComponent);
	void SetEmpty();
	FORCEINLINE FName GetItemID() { return ItemID; }
	void SetTxtStackCount(int32 InStackCount);
	void SetItemIconAsync(TSoftObjectPtr<UTexture2D> SoftIcon);
//아이콘 이미지 지정 및 비동기 함수
protected:
	
	void OnIconLoaded(FSoftObjectPath SoftIconPath);

//드래그&드랍에 필요한 함수
protected:
	//마우스 버튼 이벤트 함수
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//드래그 시작했을때 호출되는 함수
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	//드랍 했을때 호출되는 함수
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Setup")
	TSubclassOf<class USLItemSlot> DragVisualClass;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_ItemIcon;
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Cooldown;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock *Txt_StackCount;
	UPROPERTY()
	FName ItemID;
	UPROPERTY()
	int32 SlotIndex;
	UPROPERTY()
	int32 StackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class USLInventoryComponent* InventoryComponent;
};
