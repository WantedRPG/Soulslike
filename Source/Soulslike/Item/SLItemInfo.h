// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLItemInfo.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLItemInfo : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetItemName(FText InItemName);
	void SetItemIconAsync(TSoftObjectPtr<UTexture2D> SoftIcon);
private:
	void OnIconLoaded(FSoftObjectPath SoftIconPath);
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_ItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_ItemIcon;
};
