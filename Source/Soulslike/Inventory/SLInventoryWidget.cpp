// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
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

    AddItemSlots(100);

}

void USLInventoryWidget::AddItemSlots(int32 Count)
{
    for (int32 Start = 0; Start < Count; ++Start)
    {
        USLItemSlot* NewSlotWidget = CreateWidget<USLItemSlot>(this, ItemSlotWidgetClass);
        if (NewSlotWidget)
        {
            //UniformGridPanel에 자식으로 추가
            UUniformGridSlot* NewSlot = Grid_Item->AddChildToUniformGrid(NewSlotWidget);
            if (NewSlot)
            {
                // 3. 슬롯의 위치 (행/열) 설정
                // 예: 5개씩 한 줄로 배치
                NewSlot->SetRow(Start / 8);
                NewSlot->SetColumn(Start % 8);
     

                //NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
                //NewSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
                //GridSlot->SetPadding(FMargin(5.0f));
            }
        }
    }
    
}
