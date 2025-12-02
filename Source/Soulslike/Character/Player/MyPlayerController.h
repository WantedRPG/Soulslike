// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "Inventory/SLInventoryComponent.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void ToggleInventory();
	void HiddenItemText();
	void ShowItemText();
	void EquipmentWeapon();
	void UnequipmentWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CurrentCharacterContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USLPlayerHUD> PlayerHUDClass;
	UPROPERTY()
	TObjectPtr<class USLPlayerHUD> PlayerHUDInstance;
};
