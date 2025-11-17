// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SLInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlotData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStack;

	FInventorySlotData() : ItemID(NAME_None), CurrentStack(0){}
	FInventorySlotData(FName InItemID,int32 InStack) : ItemID(InItemID), CurrentStack(InStack) {}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API USLInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USLInventoryComponent();

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
	void ToggleInventory();
	UFUNCTION(BlueprintCallable)
	void DropItem(int32 InSlotIndex);
	void RequestSwapItems(int32 FromIndex, int32 ToIndex);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetInventoryWidget(class USLInventoryWidget* InWidget) {
		InventoryWidgetInstance = InWidget
			;
		InitInventoryWidget();
	}

	void UseItem(int32 InSlotIndex);
private:
	void ShowInventory();
	void HiddenInventory();
	void UpdateInventory();
	void InitInventoryWidget();
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect> ItemGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USLInventoryWidget> InventoryWidgetClass;
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
