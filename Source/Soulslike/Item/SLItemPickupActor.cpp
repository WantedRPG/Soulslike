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
	
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Script/Engine.Material'/Game/LSJ/Item/LSJHelpActorMaterial.LSJHelpActorMaterial'"));
	if (MaterialFinder.Succeeded())
	{
		ItemMaterialBillboard->AddElement(MaterialFinder.Object,nullptr,false, 32.0, 32.0, nullptr);
	}

	InteractionSphere->SetCollisionProfileName(TEXT("Item"));
}

// Called when the game starts or when spawned
void ASLItemPickupActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASLItemPickupActor::SetItemData(FName InItemID, int32 InStackCount)
{
	ItemID = InItemID;
	StackCount = InStackCount;
}

