// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLItemPickupActor.generated.h"

UCLASS()
class SOULSLIKE_API ASLItemPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLItemPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FORCEINLINE const FName& GetItemID() { return ItemID; }
	FORCEINLINE const int32 GetStackCount() { return StackCount; }
public:
	void SetItemData(FName InItemID,int32 InStackCount);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UMaterialBillboardComponent* ItemMaterialBillboard;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionSphere;

	//아이템정보
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 StackCount;
};
