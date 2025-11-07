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

	FInventorySlotData() : ItemID(NAME_None), CurrentStack(0) {}
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

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USLInventoryWidget> InventoryWidgetClassAsset;
	UPROPERTY()
	TObjectPtr<class USLInventoryWidget> InventoryWidgetInstance;

	//TArrayFInventorySlotData
	//아이템 실제 데이터

};
