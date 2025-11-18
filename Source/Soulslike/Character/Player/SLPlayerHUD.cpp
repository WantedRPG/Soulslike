// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/SLPlayerHUD.h"
#include "Inventory/SLInventoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
USLPlayerHUD::USLPlayerHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void USLPlayerHUD::NativeOnInitialized()
{

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