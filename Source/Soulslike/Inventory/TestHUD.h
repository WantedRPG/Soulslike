// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UTestHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UTestHUD(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;
	UFUNCTION(BlueprintCallable)
	class USLInventoryWidget* GetInventoryWidget();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USLInventoryWidget> WBP_Inventory;
};
