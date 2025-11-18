// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/SLInventoryComponent.h"
#include "SLPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	USLPlayerHUD(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;

	//인벤토리
public:
	UFUNCTION(BlueprintCallable)
	class USLInventoryWidget* GetInventoryWidget();

	void UpdateItemSlot(const FInventorySlotData& ChangedItemInfo);
	void AddItemSlot(int32 LastIndex, int32 InAddSlotCount);
	bool ToggleInventory();

	void SetInventoryComponent(USLInventoryComponent* InventoryComp);

	void ShowItemGetText();
	void HiddenItemGetText();
protected:
	//인벤토리 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USLInventoryWidget> WBP_Inventory;

	//아이템 획득 표시
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> Border_GetItem;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_GetItem;
};
