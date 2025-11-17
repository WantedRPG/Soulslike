// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryWidget.h"
#include "SLInventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Border.h"
#include "Item/SLItemManagerSubsystem.h"
#include "Item/SLItemData.h"
#include "Item/SLItemSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <Item/SLDragDropSlot.h>
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
USLInventoryWidget::USLInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void USLInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (Btn_All)
    {
        //Btn_All->OnClicked.AddDynamic(InventoryComponent,)
    }
}

void USLInventoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

}

void USLInventoryWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

FReply USLInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && Border_DragHandle && Border_DragHandle->IsHovered())
    {
        bIsDragging = true;
        
        
        DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
        //마우스 위치에서 위젯의 좌상단 위치를 뺀 값
        //DragOffset = InMouseEvent.GetScreenSpacePosition()- InGeometry.GetAbsolutePosition();
        return FReply::Handled().CaptureMouse(TakeWidget());
    }

    return FReply::Unhandled();
}

FReply USLInventoryWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bIsDragging)
    {
        bIsDragging = false;
        return FReply::Handled().ReleaseMouseCapture();
    }
    return FReply::Unhandled();
}

FReply USLInventoryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    
    if (bIsDragging)
    {
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this
            ->Slot))
        {
            const FGeometry& ParentGeometry = GetParent()->GetCachedGeometry
            ();
            const FVector2D NewLocalPosition = ParentGeometry.
                AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
            //Todo : 주석쓰기
            CanvasSlot->SetPosition(NewLocalPosition - DragOffset);
        }
        // 현재 마우스 위치에서 드래그 시작 시의 오프셋을 빼서 위젯의 새 위치 계산
        //SetPositionInViewport(InMouseEvent.GetScreenSpacePosition() - DragOffset,false);

        return FReply::Handled();
    }
    return FReply::Unhandled();
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
            if (ItemManager)
            {
                USLItemData* ItemData = ItemManager->GetItemData(InItemID);
                NewSlotWidget->SetItemSlotData(ItemData, InStackCount, InSlotIndex, InventoryComponent);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("아이템정보를 받아 올 수 없습니다 - USLItemManagerSubsystem = nullptr"));
            }
        }
    }
}

void USLInventoryWidget::UpdateItemSlot(int32 InSlotIndex, FName InItemID, int32 InStackCount)
{
    if (USLItemSlot* ItemSlot = Cast<USLItemSlot>(Grid_Item->GetChildAt(InSlotIndex)))
    {
        if (InItemID.IsNone())
        {
            ItemSlot->SetEmpty();
            return;
        }
        
        USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance());
        if (USLItemData* ItemData = ItemManager->GetItemData(InItemID))
        {
            ItemSlot->SetItemSlotData(ItemData, InStackCount, InSlotIndex, InventoryComponent);
        }
        //ItemSlot->UpdateStackCount(InStackCount);  
    }
}

void USLInventoryWidget::CleanInventory()
{
    Grid_Item->ClearChildren();
}

void USLInventoryWidget::SetItemStackCount(int32 InSlotIndex, int32 InStackCount)
{
    if (USLItemSlot* ItemSlot = Cast<USLItemSlot>(Grid_Item->GetChildAt(InSlotIndex)))
    {
        ItemSlot->SetTxtStackCount(InStackCount);
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

void USLInventoryWidget::SetInventoryComponent(USLInventoryComponent* InInventoryCompoent)
{
    InventoryComponent = InInventoryCompoent;
}
