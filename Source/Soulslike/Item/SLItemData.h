// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SLItemEums.h"
#include "FItemTableRow.h"
#include "SLItemData.generated.h"

/**
 * 
 */



UCLASS()
class SOULSLIKE_API USLItemData : public UDataAsset
{
	GENERATED_BODY()
public:
	USLItemData() : ItemID(), ItemType(EItemType::None), ItemName(), ItemDescription(), MaxStack(1) {}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ItemTags;
	//아이템 효과
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EItemActionType, FItemActionDetail> ItemActionMap;
	//월드 아이템 BP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WordItemBP")
	TObjectPtr<class AItemActor> WordItemClass;
};
