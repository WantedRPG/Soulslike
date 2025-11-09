// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryComponent.h"
#include "SLInventoryWidget.h"
#include <Item/SLItemManagerSubsystem.h>
#include <Item/SLItemData.h>

// Sets default values for this component's properties
USLInventoryComponent::USLInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USLInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (InventoryWidgetClassAsset)
	{
		InventoryWidgetInstance = CreateWidget<USLInventoryWidget>(GetWorld(), InventoryWidgetClassAsset);
		if (InventoryWidgetInstance)
		{
			InventoryWidgetInstance->AddToViewport();
			InventoryWidgetInstance->AddItemSlots(MaxItemSlotCount);
		}
	}
}


// Called every frame
void USLInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USLInventoryComponent::ShowItemGetUI()
{
	check(InventoryWidgetInstance);

	InventoryWidgetInstance->ShowGetItemUI();
}

void USLInventoryComponent::HiddenItemGetUI()
{
	check(InventoryWidgetInstance);
	InventoryWidgetInstance->HiddenGetItemUI();
}

void USLInventoryComponent::AddItem(FName InItemID,int32 InStackCount)
{
	check(InventoryWidgetInstance);
	USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance());

	if (USLItemData* ItemData = ItemManager->GetItemData(InItemID))
	{
		InventoryWidgetInstance->AddItemSlot(ItemData, InStackCount);
	}

	
}

void USLInventoryComponent::InitInventoryWidget()
{
	check(InventoryWidgetInstance);

	InventoryWidgetInstance->AddItemSlots(MaxItemSlotCount);
}



