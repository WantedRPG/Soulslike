// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemSlot.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SLItemData.h"
#include "SLDragDropSlot.h"
#include "Inventory/SLInventoryComponent.h"
#include "Item/SLItemData.h"
#include "SLItemManagerSubsystem.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Styling/SlateBrush.h"
#include <Blueprint/WidgetBlueprintLibrary.h>


USLItemSlot::USLItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}
void USLItemSlot::SetItemSlotData(USLItemData* NewItemData,int32 InStackCount,int32 InSlotIndex, USLInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;
	SlotIndex = InSlotIndex;
	StackCount = InStackCount;

	if (nullptr == NewItemData)
	{
		return;
	}

	ItemID = NewItemData->ItemID;
	FGameplayTag StackableTag = FGameplayTag::RequestGameplayTag(FName("Item.Property.Stackable"));
	//같은 종류 아이템 누적 가능여부 tag로 구별 
	if (NewItemData->ItemTags.HasTag(StackableTag))
	{
		Txt_StackCount->SetVisibility(ESlateVisibility::Visible);
		Txt_StackCount->SetText(FText::AsNumber(InStackCount));
	}

	SetItemIconAsync(NewItemData->ItemIcon);
}

void USLItemSlot::SetEmpty()
{
	ItemID = NAME_None;
	Image_ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	Txt_StackCount->SetVisibility(ESlateVisibility::Hidden);
}

void USLItemSlot::SetTxtStackCount(int32 InStackCount)
{
	Txt_StackCount->SetText(FText::AsNumber(InStackCount));
	Txt_StackCount->SetVisibility(ESlateVisibility::Visible);
}

void USLItemSlot::SetItemIconAsync(TSoftObjectPtr<UTexture2D> SoftIcon)
{
	//Todo : 이미지 로드 관련해서 조사하기, 위젯 닫을때 메모리해제 안되는거같음 -> 이미지 겹침 문제
	UTexture2D* LoadedTexture = SoftIcon.Get();
	//메모리에 로드되어 있는지 확인
	//Slot이 가리는거같음
	if (LoadedTexture && Image_ItemIcon)
	{
		Image_ItemIcon->SetBrushFromTexture(LoadedTexture);
		Image_ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		//로드되어 있지 않다면, 비동기 로드 요청
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad(
			SoftIcon.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &USLItemSlot::OnIconLoaded, SoftIcon.ToSoftObjectPath())
		);
	}
}

void USLItemSlot::OnIconLoaded(FSoftObjectPath SoftIconPath)
{
	// 로드가 완료된 후 텍스처 적용
	UTexture2D* LoadedTexture = Cast<UTexture2D>(SoftIconPath.ResolveObject());

	if (LoadedTexture && Image_ItemIcon)
	{
		Image_ItemIcon->SetVisibility(ESlateVisibility::Visible);
		Image_ItemIcon->SetBrushFromTexture(LoadedTexture);
	}
}

FReply USLItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;

	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (ItemID.IsNone())
		return Reply.NativeReply;
	// 좌클릭 입력이 들어온 경우
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		
	}

	return Reply.NativeReply;
}

void USLItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	if (OutOperation == nullptr)
	{
		// 드래그 슬롯을 생성
		if (USLDragDropSlot* Operation = NewObject<USLDragDropSlot>())
		{
			OutOperation = Operation;

			// 슬롯 인덱스 지정
			Operation->PrevSlotIndex = SlotIndex;
			Operation->InventoryComponent = InventoryComponent;
			// 드래그 슬롯의 드래그 위젯을 설정.
			USLItemSlot* DragVisualWidget = CreateWidget<USLItemSlot>(GetOwningPlayer(), DragVisualClass);
			if (DragVisualWidget)
			{
				USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance());
				if (nullptr == ItemManager)
					return;
				DragVisualWidget->OnIconLoaded(ItemManager->GetItemData(ItemID)->ItemIcon.Get());
				DragVisualWidget->SetTxtStackCount(StackCount);
				Operation->DefaultDragVisual = DragVisualWidget;
			}
		}
	}
}

bool USLItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("out %d"), SlotIndex);
	USLDragDropSlot* Operation = Cast<USLDragDropSlot>(InOperation);

	if (Operation)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d %d"), SlotIndex, Operation->PrevSlotIndex);
		Operation->InventoryComponent->RequestSwapItems(SlotIndex, Operation->PrevSlotIndex);

		// 같은 타입의 슬롯인 경우
		return true;
	}
	
	return false;
}

FReply  USLItemSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 어느 마우스 버튼이 더블클릭되었는지 확인할 수 있습니다.
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Left Mouse Button Double Click!"));
		// 아이템 사용/장착 로직 호출
	}

	// 이벤트를 처리했음을 반환하여 다른 위젯으로 전파되지 않게 합니다.
	return FReply::Handled();
}

void USLItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}
