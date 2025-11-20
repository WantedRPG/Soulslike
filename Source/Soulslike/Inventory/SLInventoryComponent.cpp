// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryComponent.h"
#include "SLInventoryWidget.h"
#include <Item/SLItemManagerSubsystem.h>
#include <Item/SLItemData.h>
#include <Item/SLItemPickupActor.h>
#include "Item/GA/ItemDataObject.h"


#include <AbilitySystemBlueprintLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
USLInventoryComponent::USLInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryWidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LSJ/Inventory/WBP_Inventory.WBP_Inventory_C'"));
	if (InventoryWidgetFinder.Succeeded())
	{
		InventoryWidgetClass = InventoryWidgetFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidgetClassAsset를 찾을 수 없습니다! 경로를 확인하세요."));
	}

	static ConstructorHelpers::FClassFinder<ASLItemPickupActor> ItemActorFinder(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Item/BP_PickupItem.BP_PickupItem_C'"));
	if (ItemActorFinder.Succeeded())
	{
		ItemPickupActorClass = ItemActorFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USLInventoryWidget를 찾을 수 없습니다! 경로를 확인하세요."));
	}
}


// Called when the game starts
void USLInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void USLInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// 아이템 스택 변경 후 UI 업데이트 및 델리게이트 호출을 처리하는 헬퍼 함수
void USLInventoryComponent::UpdateItemAndBroadcast(int32 SlotIndex, int32 NewStackCount, const FName& ItemID)
{
	Items[SlotIndex].SlotIndex = SlotIndex;
	Items[SlotIndex].ItemID = ItemID;
	Items[SlotIndex].CurrentStack = NewStackCount;
	//Todo : 숫자만 업데이트하는 델리게이트 필요
	if (OnInventoryUpdated.IsBound())
	{
		OnInventoryUpdated.Broadcast(Items[SlotIndex]);
	}
	if (QuickItemSlotIndex == SlotIndex)
	{
		if (OnQuickItemUpdated.IsBound())
		{
			OnQuickItemUpdated.Broadcast(Items[SlotIndex].ItemID, Items[SlotIndex].CurrentStack, SlotIndex);
		}
	}
}

// 아이템 스택 변경 후 UI 업데이트 및 델리게이트 호출을 처리하는 헬퍼 함수
void USLInventoryComponent::AddItemAndBroadcast(int32 SlotIndex, int32 NewStackCount, const FName& ItemID)
{
	FInventorySlotData Info;
	Info.SlotIndex = SlotIndex;
	Info.ItemID = ItemID;
	Info.CurrentStack = NewStackCount;
	Items.Add(Info);

	if (OnInventoryUpdated.IsBound())
	{
		OnInventoryUpdated.Broadcast(Info);
	}
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
		UE_LOG(LogTemp, Warning, TEXT("ItemID %s 존재하지 않는 아이템입니다"),*InItemID.ToString());
		return false;
	}

	//처음에 획득하는 아이템을 제외하고 맵에서 획득할때 알려야 함
	
	FGameplayTag StackableTag = FGameplayTag::RequestGameplayTag(FName("Item.Property.Stackable"));
	//같은 종류 아이템 누적 가능여부 tag로 구별 
	bool bStackTag = InItemData->ItemTags.HasTag(StackableTag);
	
	int32 MaxStack = InItemData->MaxStack;

	//기존 아이템과 합치기
	if (bStackTag)
	{
		for (int32 Index = 0; Index < Items.Num(); ++Index)
		{
			//ItemID 일치하고, 비교 아이템의 StackCount가 MaxStackCount보다 낮다면 아이템을 넣고 초과된 수만큼 아이템 넣기
			if (!InItemID.IsNone() && InItemID.IsEqual(Items[Index].ItemID) && MaxStack > Items[Index].CurrentStack)
			{
				int32 TotalStackCount = Items[Index].CurrentStack + InStackCount;
				if (TotalStackCount <= MaxStack)
				{
					// 스택이 모두 들어가는 경우
					UpdateItemAndBroadcast(Index, TotalStackCount, InItemID);

					return true;
				}
				else
				{
					// 스택이 넘치는 경우
					UpdateItemAndBroadcast(Index, MaxStack, InItemID);
					InStackCount = TotalStackCount - MaxStack;
				}
			}
		}
	}

	// 빈 슬롯에 아이템 추가 로직
	if (InStackCount > 0)
	{
		for (int32 Index = 0; Index < Items.Num(); ++Index)
		{
			if (Items[Index].ItemID.IsNone())
			{
				//이 슬롯에 추가할 아이템 수량 결정
				int32 AmountToAdd = FMath::Min(InStackCount, InItemData->MaxStack);

				// 데이터 업데이트 및 UI/델리게이트 알림
				UpdateItemAndBroadcast(Index, AmountToAdd, InItemID);
				++CurrentItemCount; 

				// 남은 수량 갱신
				InStackCount -= AmountToAdd;

				// 추가할 아이템이 더 이상 없으면 루프 종료
				if (InStackCount <= 0)
				{
					break;
				}
			}
		}
	}
	return true;
}

void USLInventoryComponent::InitInventory(int32 InMaXSlotCount)
{
	ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance());
	check(ItemManager);
	MaxItemSlotCount = InMaXSlotCount;
	//아이템 슬롯크기에 맞게 Item 생성
	for (int32 Index = 0; Index < MaxItemSlotCount; ++Index)
	{
		Items.Add(FInventorySlotData());
	}
	if (AddInventorySlot.IsBound())
		AddInventorySlot.Broadcast(0,InMaXSlotCount);
}

void USLInventoryComponent::RequestSwapItems(int32 FromIndex, int32 ToIndex)
{
	//인덱스 유효성 검사
	if (!Items.IsValidIndex(FromIndex) || !Items.IsValidIndex(ToIndex))
		return;

	//대상 슬롯이 비어있는지 확인
	if (Items[ToIndex].ItemID.IsNone())
	{
		Items[ToIndex] = Items[FromIndex];
		Items[FromIndex].ItemID = NAME_None;
		Items[FromIndex].CurrentStack = 0;
	}
	else
	{
		FInventorySlotData TempItemData = Items[ToIndex];
		Items[ToIndex] = Items[FromIndex];
		Items[FromIndex] = TempItemData;
	}

	UpdateItemAndBroadcast(ToIndex, Items[ToIndex].CurrentStack, Items[ToIndex].ItemID);
	UpdateItemAndBroadcast(FromIndex, Items[FromIndex].CurrentStack, Items[FromIndex].ItemID);
}

void USLInventoryComponent::DropItem(int32 InSlotIndex)
{
	if (ItemPickupActorClass)
	{
		if (ASLItemPickupActor* ItemPickupActor = GetWorld()->SpawnActorDeferred<ASLItemPickupActor>(ItemPickupActorClass, GetOwner()->GetTransform()))
		{
			ItemPickupActor->SetItemData(Items[InSlotIndex].ItemID, Items[InSlotIndex].CurrentStack);
			UGameplayStatics::FinishSpawningActor(ItemPickupActor, GetOwner()->GetTransform());
		}
		
	}
	--CurrentItemCount;
	UpdateItemAndBroadcast(InSlotIndex, 0, NAME_None);
}

void USLInventoryComponent::UseItem(int32 InSlotIndex)
{
	if (nullptr == ItemManager)
		return;

	AActor* OwnerActor = GetOwner();

	// OwnerActor를 대상으로 이벤트를 보냄
	FGameplayEventData EventData;
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Item.Event.Consumable"));
	EventData.Instigator = OwnerActor;
	EventData.Target = OwnerActor;
	UItemDataObject* ItemDataObject = NewObject<UItemDataObject>();
	ItemDataObject->ItemID = Items[InSlotIndex].ItemID;
	Items[InSlotIndex].CurrentStack -= 1;
	if (Items[InSlotIndex].CurrentStack == 0)
	{
		--CurrentItemCount;
	}
	EventData.OptionalObject = ItemDataObject;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, EventData.EventTag, EventData);

	UpdateItemAndBroadcast(InSlotIndex,Items[InSlotIndex].CurrentStack,Items[InSlotIndex].ItemID);
}

void USLInventoryComponent::SetQuickSlotItem(int32 InSlotIndex)
{
	if (OnQuickItemUpdated.IsBound())
	{
		OnQuickItemUpdated.Broadcast(Items[InSlotIndex].ItemID, Items[InSlotIndex].CurrentStack,InSlotIndex);
		QuickItemSlotIndex = InSlotIndex;
	}
}

