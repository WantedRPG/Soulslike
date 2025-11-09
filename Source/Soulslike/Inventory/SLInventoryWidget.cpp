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

	Grid_Item->ClearChildren();
    
}

USLItemSlot* USLInventoryWidget::FindAvailableSlotIndex() const
{
    
    for (int32 Index = 0; Index < Grid_Item->GetChildrenCount(); ++Index)
    {
        if (USLItemSlot* ItemSlot = Cast<USLItemSlot>(Grid_Item->GetChildAt(Index)))
        {
            if (ItemSlot->GetItemID().IsNone())
            {
                return ItemSlot;
            }
        }
    }

    return nullptr;
}

void USLInventoryWidget::AddItemSlots(int32 Count)
{
    for (int32 Start = 0; Start < Count; ++Start)
    {
        USLItemSlot* NewSlotWidget = CreateWidget<USLItemSlot>(this, ItemSlotWidgetClass);
        if (NewSlotWidget)
        {
            //NewSlotWidget->SetItemData()
            
            //UniformGridPanel에 자식으로 추가
            UUniformGridSlot* NewSlot = Grid_Item->AddChildToUniformGrid(NewSlotWidget);
            if (NewSlot)
            {
                // 3. 슬롯의 위치 (행/열) 설정
                // 예: 5개씩 한 줄로 배치
                NewSlot->SetRow(Start / MaxRow);
                NewSlot->SetColumn(Start % MaxRow);

                //NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
                //NewSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
                //GridSlot->SetPadding(FMargin(5.0f));
            }
        }
    }
    //Grid_Item->GetChild
}
bool USLInventoryWidget::AddItemSlot(USLItemData* InItemData, int32 InStackCount)
{
    if (USLItemSlot* ItemSlot = FindAvailableSlotIndex())
    {
        ItemSlot->SetItemData(InItemData, InStackCount);
        return true;
    }
        
  
    return false;
    //빈공간에 넣어야하는데 탐색을 어떻게 해야하나?
    //중간이 빈공간이라면?
}
void USLInventoryWidget::ShowGetItemUI()
{
    Border_GetItem->SetVisibility(ESlateVisibility::Visible);
}

void USLInventoryWidget::HiddenGetItemUI()
{
    Border_GetItem->SetVisibility(ESlateVisibility::Hidden);
}