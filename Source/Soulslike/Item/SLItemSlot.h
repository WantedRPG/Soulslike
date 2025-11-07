// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	USLItemSlot(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;

public:
	void SetItemData(class USLItemData* NewItemData);
protected:
	void SetItemIconAsync(TSoftObjectPtr<UTexture2D> SoftIcon);
	void OnIconLoaded(TSoftObjectPtr<UTexture2D> LoadedIcon);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_ItemIcon;
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Cooldown;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Slot;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock *Txt_StackCount;
	UPROPERTY()
	FName ItemID;
};
