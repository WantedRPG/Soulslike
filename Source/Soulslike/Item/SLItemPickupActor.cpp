// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/MaterialBillboardComponent.h"
// Sets default values
ASLItemPickupActor::ASLItemPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	SetRootComponent(InteractionSphere);
	ItemMaterialBillboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("ItemMaterialBillboard"));
	ItemMaterialBillboard->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASLItemPickupActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASLItemPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASLItemPickupActor::SetItemData(FName InItemID, int32 InStackCount)
{
	ItemID = InItemID;
	StackCount = InStackCount;
}

