// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/SLPlayerHUD.h"
#include "Inventory/SLInventoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Item/SLItemInfo.h"
#include "Item/SLItemManagerSubsystem.h"
#include "Item/SLItemData.h"
#include "Item/SLItemSlot.h"

USLPlayerHUD::USLPlayerHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void USLPlayerHUD::NativeOnInitialized()
{
    GetWorld()->GetTimerManager();
    
    //QuickSlot_Weapon->SetEmpty();
    QuickSlot_Consumable->SetEmpty();
}


void USLPlayerHUD::HiddeItemInfo()
{
    if (WBP_ItemInfo)
    {
        WBP_ItemInfo->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void USLPlayerHUD::ShowItemInfo(FName InItemID)
{
    if (WBP_ItemInfo)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            World->GetTimerManager().ClearTimer(ItemInfoTimerHandle);
            World->GetTimerManager().SetTimer(
                ItemInfoTimerHandle,
                [this]()
                {
                    HiddeItemInfo();
                },
                3.0f, // 3초 후 실행
                false // 반복 안 함
            );
        }

        if (USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance()))
        {
            if (USLItemData* ItemData = ItemManager->GetItemData(InItemID))
            {
                WBP_ItemInfo->SetItemName(ItemData->ItemName);
                WBP_ItemInfo->SetItemIconAsync(ItemData->ItemIcon);
                WBP_ItemInfo->SetVisibility(ESlateVisibility::Visible);
            }
        }
    }
}

void USLPlayerHUD::ShowItemGetText()
{
	if(Border_GetItem)
		Border_GetItem->SetVisibility(ESlateVisibility::Visible);
	if (Txt_GetItem)
		Txt_GetItem->SetVisibility(ESlateVisibility::Visible);
}
void USLPlayerHUD::HiddenItemGetText()
{
	if (Border_GetItem)
		Border_GetItem->SetVisibility(ESlateVisibility::Hidden);
	if (Txt_GetItem)
		Txt_GetItem->SetVisibility(ESlateVisibility::Hidden);
}
USLInventoryWidget* USLPlayerHUD::GetInventoryWidget()
{
	return WBP_Inventory;
}

void USLPlayerHUD::UpdateItemSlot(const FInventorySlotData& ChangedItemInfo)
{
    WBP_Inventory->UpdateItemSlot(ChangedItemInfo.SlotIndex, ChangedItemInfo.ItemID, ChangedItemInfo.CurrentStack);
}

void USLPlayerHUD::AddItemSlot(int32 LastIndex, int32 InAddSlotCount)
{
    WBP_Inventory->AddEmptySlot(LastIndex, InAddSlotCount);
}

bool USLPlayerHUD::ToggleInventory()
{
    if (WBP_Inventory)
    {
        if (WBP_Inventory->GetVisibility() == ESlateVisibility::Collapsed)
        {
            WBP_Inventory->SetVisibility(ESlateVisibility::Visible);
            return true;
        }
        else
        {
            WBP_Inventory->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    return false;
}

void USLPlayerHUD::SetInventoryComponent(USLInventoryComponent* InventoryComp)
{
    if (WBP_Inventory)
    {
        WBP_Inventory->SetInventoryComponent(InventoryComp);
    }
}

void USLPlayerHUD::EquipWeapon()
{
    if (nullptr==QuickSlot_Weapon)
        return;
    if (USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance()))
    {
        if (USLItemData* ItemData = ItemManager->GetItemData(FName(TEXT("Item_010"))))
        {
            QuickSlot_Weapon->SetItemSlotData(ItemData,1,0);
        }
    }
}

void USLPlayerHUD::UnequipWeapon()
{
    if (nullptr == QuickSlot_Weapon)
        return;
    QuickSlot_Weapon->SetEmpty();
}

void USLPlayerHUD::UpdateQuickItem(FName InItemID,int32 InStackCount,int32 InSlotCount)
{
    if (nullptr == QuickSlot_Consumable)
        return;
    if (USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance()))
    {
        if (USLItemData* ItemData = ItemManager->GetItemData(InItemID))
        {
            QuickSlot_Consumable->SetItemSlotData(ItemData, InStackCount, InSlotCount);
        }
    }
}