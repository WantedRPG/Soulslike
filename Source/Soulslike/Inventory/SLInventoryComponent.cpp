// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryComponent.h"
#include "SLInventoryWidget.h"
#include <Item/SLItemManagerSubsystem.h>
#include <Item/SLItemData.h>
#include <Item/SLItemPickupActor.h>

#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
USLInventoryComponent::USLInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryWidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LSJ/Inventory/WBP_Inventory.WBP_Inventory_C'"));
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
	
	FGameplayTag StackableTag = FGameplayTag::RequestGameplayTag(FName("Item.Property.Stackable"));
	//같은 종류 아이템 누적 가능여부 tag로 구별 
	if (InItemData->ItemTags.HasTag(StackableTag))
	{
		for (int32 Index = 0; Index<Items.Num(); ++Index)
		{
			//ItemID 일치하고, 비교 아이템의 StackCount가 MaxStackCount보다 낮다면 아이템을 넣고 초과된 수만큼 아이템 넣기
			if (InItemID.IsEqual(Items[Index].ItemID) && InItemData->MaxStack > Items[Index].CurrentStack)
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
					//101 21
					//20 
					//121
					
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

	//빈 슬롯에 아이템 추가
	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		if (Items[Index].ItemID.IsNone())
		{
			if (InStackCount > 0)
			{
				Items[Index].ItemID = InItemID;
				//초과된 StackCount 고려
				Items[Index].CurrentStack = FMath::Min(InStackCount, InItemData->MaxStack);
				InStackCount -= InItemData->MaxStack;

				//UI업데이트
				if (InventoryWidgetInstance)
				{
					InventoryWidgetInstance->UpdateItemSlot(Index, Items[Index].ItemID, Items[Index].CurrentStack);
				}

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
	check(InventoryWidgetInstance);
	InventoryWidgetInstance->SetInventoryComponent(this);
	UpdateInventory();
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

	InventoryWidgetInstance->UpdateItemSlot(ToIndex, Items[ToIndex].ItemID, Items[ToIndex].CurrentStack);
	InventoryWidgetInstance->UpdateItemSlot(FromIndex, Items[FromIndex].ItemID, Items[FromIndex].CurrentStack);
}

void USLInventoryComponent::ToggleInventory()
{
	if (InventoryWidgetInstance->GetVisibility()==ESlateVisibility::Visible)
	{
		HiddenInventory();
	}
	else
	{
		ShowInventory();
	}
}

void USLInventoryComponent::DropItem(int32 InSlotIndex)
{
	if (ItemPickupActorClass)
	{
		ASLItemPickupActor* ItemPickupActor =GetWorld()->SpawnActor<ASLItemPickupActor>(ItemPickupActorClass, GetOwner()->GetTransform());
		ItemPickupActor->SetItemData(Items[InSlotIndex].ItemID, Items[InSlotIndex].CurrentStack);
	}
	Items[InSlotIndex].ItemID = NAME_None;
	Items[InSlotIndex].CurrentStack = 0;
	InventoryWidgetInstance->SetEmptySlot(InSlotIndex);
}

void USLInventoryComponent::UseItem(int32 InSlotIndex)
{
	if (nullptr == ItemManager || nullptr == ItemGameplayEffectClass)
		return;

	USLItemData* ItemData = ItemManager->GetItemData(Items[InSlotIndex].ItemID);
	if (nullptr == ItemData)
		return;

	for (auto& GEInfo : ItemData->ItemActionMap[EItemActionType::Primary].DataDrivenModifierInfos)
	{
		/*
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectDetail.GameplayEffectClass, 1.0f, AbilitySystemComponent->MakeEffectContext());
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(GEInfo.ItemTag, GEInfo.Value);
			if (EffectDetail.Duration > 0)
			{
				SpecHandle.Data->SetDuration(GEInfo.Duration, true);
			}
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		*/
	}


	/*
	// 1. 컴포넌트 소유 액터로부터 ASC 가져오기
	UAbilitySystemComponent* ASC = nullptr;
	if (AActor* Owner = GetOwner())
	{
		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Owner);
		if (ASI)
		{
			ASC = ASI->GetAbilitySystemComponent();
		}
	}

	if (!ASC || !HealingEffectClass || !ItemDataTable) return;

	// 2. 데이터 테이블에서 회복량 가져오기
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemRowName, TEXT("UseItem Logic"));
	if (!ItemData) return;

	float HealingAmount = ItemData->HealingAmount;

	// 3. Gameplay Effect Spec 생성
	// GA 없이 직접 생성하므로, 레벨(1.0f)과 ContextHandle을 직접 제공합니다.
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(HealingEffectClass, 1.0f, EffectContext);

	if (!SpecHandle.IsValid()) return;

	// 4. SetByCaller를 사용하여 동적 값 주입
	FGameplayTag HealingTag = UGameplayTagsManager::Get().RequestGameplayTag(TEXT("Data.HealingValue"));
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(HealingTag, HealingAmount);

	// 5. GE 적용 (GA를 거치지 않고 바로 적용)
	// 소유자에게 직접 Spec을 적용합니다.
	FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	*/
}
void USLInventoryComponent::ShowInventory()
{
	//InventoryWidgetInstance = CreateWidget<USLInventoryWidget>(GetWorld(), InventoryWidgetClass);

	if (InventoryWidgetInstance)
	{
		//InventoryWidgetInstance->AddToViewport();
		InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		
		//InitInventoryWidget();
		//마우스 UI모드 및 보이기
		if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
		{
			// 게임 월드와 상호작용도 필요
			FInputModeGameAndUI InputMode; 
			// 포커스 설정
			InputMode.SetWidgetToFocus(InventoryWidgetInstance->TakeWidget());
			
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void USLInventoryComponent::HiddenInventory()
{
	//InventoryWidgetInstance->RemoveFromParent();
	//InventoryWidgetInstance = nullptr;
	InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		//마우스 게임모드 및 감추기
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}



