// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryComponent.h"
#include "SLInventoryWidget.h"

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
		}
	}
}


// Called every frame
void USLInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

