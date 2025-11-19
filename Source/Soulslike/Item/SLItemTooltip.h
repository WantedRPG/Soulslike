// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLItemTooltip.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLItemTooltip : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetupDragVisualIcon(UTexture2D* LoadedTexture);

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_ItemIcon;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_ItemName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_ItemDesc;
};
