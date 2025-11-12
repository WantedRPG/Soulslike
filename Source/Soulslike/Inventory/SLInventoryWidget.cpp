// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Border.h"
#include "Item/SLItemManagerSubsystem.h"
#include "Item/SLItemData.h"
#include "Item/SLItemSlot.h"

USLInventoryWidget::USLInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void USLInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USLInventoryWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void USLInventoryWidget::AddItemSlot(int32 InSlotIndex, FName InItemID, int32 InStackCount)
{
    USLItemSlot* NewSlotWidget = CreateWidget<USLItemSlot>(this, ItemSlotWidgetClass);
    if (NewSlotWidget)
    {
        //UniformGridPanel에 자식으로 추가
        //AddChildToUniformGrid 한 후 슬롯 데이터가 적용됨
        UUniformGridSlot* NewSlot = Grid_Item->AddChildToUniformGrid(NewSlotWidget);
        if (NewSlot)
        {   
            NewSlot->SetRow(InSlotIndex / MaxRow);
            NewSlot->SetColumn(InSlotIndex % MaxRow);

            //아이템 정보를 ItemManager에서 가져와서 슬롯데이터 초기화
            USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance());
            if (USLItemData* ItemData = ItemManager->GetItemData(InItemID))
            {
                NewSlotWidget->SetItemData(ItemData, InStackCount);
            }
        }
    }
}

void USLInventoryWidget::UpdateItemSlot(int32 InSlotIndex, FName InItemID, int32 InStackCount)
{
    if (USLItemSlot* ItemSlot = Cast<USLItemSlot>(Grid_Item->GetChildAt(InSlotIndex)))
    {
        if (ItemSlot->GetItemID() != InItemID)
        {
            USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance());
            if (USLItemData* ItemData = ItemManager->GetItemData(InItemID))
            {
                ItemSlot->SetItemData(ItemData, InStackCount);
            }
        }
        ItemSlot->UpdateStackCount(InStackCount);  
    }
}

void USLInventoryWidget::ShowGetItemUI()
{
    Border_GetItem->SetVisibility(ESlateVisibility::Visible);
}

void USLInventoryWidget::HiddenGetItemUI()
{
    Border_GetItem->SetVisibility(ESlateVisibility::Hidden);
}

void USLInventoryWidget::CleanInventory()
{
    Grid_Item->ClearChildren();
}

void USLInventoryWidget::SetItemStackCount(int32 InSlotIndex, int32 InStackCount)
{
    if (USLItemSlot* ItemSlot = Cast<USLItemSlot>(Grid_Item->GetChildAt(InSlotIndex)))
    {
        ItemSlot->UpdateStackCount(InStackCount);
    }
}

void USLInventoryWidget::SetEmptySlot(int32 InSlotIndex)
{
    if (USLItemSlot* ItemSlot = Cast<USLItemSlot>(Grid_Item->GetChildAt(InSlotIndex)))
    {
        ItemSlot->SetEmpty();
    }
}

void USLInventoryWidget::SetRemoveSlot(int32 InSlotIndex)
{

}