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

	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryWidgetFinder(TEXT("WidgetBlueprint'/Game/LSJ/Inventory/WBP_Inventory.WBP_Inventory_C'"));
	if (InventoryWidgetFinder.Succeeded())
	{
		InventoryWidgetClass = InventoryWidgetFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidgetClassAsset를 찾을 수 없습니다! 경로를 확인하세요."));
	}
}


// Called when the game starts
void USLInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance());

	//보유 아이템 데이터 초기세팅
	for (int32 Index = 0; Index < MaxItemSlotCount; ++Index)
	{
		Items.Add(FInventorySlotData());
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

bool USLInventoryComponent::AddItem(FName InItemID,int32 InStackCount)
{
	//인벤토리가 가득 찼는지 체크
	if (CurrentItemCount >= MaxItemSlotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("인벤토리 가득 참"));
		return false;
	}

	const USLItemData* InItemData = ItemManager->GetItemData(InItemID);
	//아이템 존재 확인
	if (nullptr == InItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("존재하지 않는 아이템입니다"));
		return false;
	}
	
	FGameplayTag StackableTag = FGameplayTag::RequestGameplayTag(FName("Item.Property.Stackable"));
	//같은 종류 아이템 누적 가능여부 tag로 구별 
	if (InItemData->ItemTags.HasTag(StackableTag))
	{
		for (int32 Index = 0; Index<Items.Num(); ++Index)
		{
			//ItemID 일치하고, 비교 아이템의 StackCount가 MaxStackCount보다 낮다면 아이템을 넣고 초과된 수만큼 아이템 넣기
			if (InItemID.IsEqual(Items[Index].ItemID) && InItemData->MaxStack < Items[Index].CurrentStack)
			{
				int32 TotalStackCount = Items[Index].CurrentStack + InStackCount;
				if (TotalStackCount <= InItemData->MaxStack)
				{
					Items[Index].CurrentStack = TotalStackCount;
					//UI업데이트
					if (InventoryWidgetInstance)
					{
						InventoryWidgetInstance->SetItemStackCount(Index, Items[Index].CurrentStack);
					}
					
					
					return true;
				}
				else
				{
					Items[Index].CurrentStack = InItemData->MaxStack;
					//UI업데이트
					if (InventoryWidgetInstance)
					{
						InventoryWidgetInstance->SetItemStackCount(Index, Items[Index].CurrentStack);
					}
					InStackCount = TotalStackCount - InItemData->MaxStack;
				}
			}
		}
	}

	//아이템 추가
	//초과된 StackCount 고려
	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		if (Items[Index].ItemID.IsNone())
		{
			if (InStackCount > 0)
			{
				Items[Index].ItemID = InItemID;
				Items[Index].CurrentStack = FMath::Min(InStackCount, InItemData->MaxStack);
				//UI업데이트
				if (InventoryWidgetInstance)
				{
					InventoryWidgetInstance->UpdateItemSlot(Index, Items[Index].ItemID, Items[Index].CurrentStack);
				}
				
				InStackCount -= InItemData->MaxStack;
				++CurrentItemCount;
			}
			else
				break;
		}
	}
	
	return true;
}

void USLInventoryComponent::UpdateInventory()
{
	check(InventoryWidgetInstance);
	InventoryWidgetInstance->CleanInventory();
	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		InventoryWidgetInstance->AddItemSlot(Index,Items[Index].ItemID, Items[Index].CurrentStack);
	}
}

void USLInventoryComponent::InitInventoryWidget()
{

	UpdateInventory();
}

void USLInventoryComponent::ToggleInventory()
{
	if (InventoryWidgetInstance)
	{
		HiddenInventory();
	}
	else
	{
		ShowInventory();
	}
}

void USLInventoryComponent::ShowInventory()
{
	InventoryWidgetInstance = CreateWidget<USLInventoryWidget>(GetWorld(), InventoryWidgetClass);

	if (InventoryWidgetInstance)
	{
		InventoryWidgetInstance->AddToViewport();
		InitInventoryWidget();
		//마우스 UI모드 및 보이기
		if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
		{
			//PlayerController->SetInputMode(FInputModeUIOnly());
			//PlayerController->bShowMouseCursor = false;
		}
	}
}

void USLInventoryComponent::HiddenInventory()
{
	InventoryWidgetInstance->RemoveFromParent();
	InventoryWidgetInstance = nullptr;
	
	//마우스 게임모드 및 감추기
	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		//PlayerController->SetInputMode(FInputModeGameOnly());
		//PlayerController->bShowMouseCursor = false;
	}
}



