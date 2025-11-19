// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/SLItemEums.h"
#include "SLInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlotData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	FInventorySlotData() : SlotIndex(0),ItemID(NAME_None), CurrentStack(0), ItemType(EItemType::None){}
	FInventorySlotData(FName InItemID,int32 InStack) : SlotIndex(0),ItemID(InItemID), CurrentStack(InStack), ItemType(EItemType::None) {}
};

// 데이터 변경 시 호출될 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const FInventorySlotData&, ChangeInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddInventorySlot, int32, LastSlotIndex, int32 , AddSlotCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAcquired, FName, ItemID);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API USLInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	USLInventoryComponent();

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryUpdated OnInventoryUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FAddInventorySlot AddInventorySlot;
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemAcquired OnItemAcquired;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(FName InItemID, int32 InStackCount);
	//인벤토리 widget 켜기/끄기
	UFUNCTION(BlueprintCallable)
	void DropItem(int32 InSlotIndex);
	void RequestSwapItems(int32 FromIndex, int32 ToIndex);
	void InitInventory(int32 InMaXSlotCount);
	void AddItem();
	void UseItem(int32 InSlotIndex);
private:
	void UpdateItemAndBroadcast(int32 SlotIndex, int32 NewStackCount, const FName& ItemID);
	void AddItemAndBroadcast(int32 SlotIndex, int32 NewStackCount, const FName& ItemID);
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> InventoryWidgetClass;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USLInventoryWidget> InventoryWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASLItemPickupActor> ItemPickupActorClass;
	//최대 아이템 칸
	UPROPERTY(EditDefaultsOnly)
	int32 MaxItemSlotCount = 100;
	//현재 아이템 칸
	UPROPERTY(VisibleAnywhere)
	int32 CurrentItemCount = 0;

	//아이템 실제 데이터
	UPROPERTY(VisibleAnywhere)
	TArray<FInventorySlotData> Items;
	//아이템 매니저
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USLItemManagerSubsystem> ItemManager;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class USLInventoryComponent* InventoryComponent;
};
