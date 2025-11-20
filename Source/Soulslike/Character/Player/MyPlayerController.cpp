// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "SLPlayerHUD.h"
#include "Inventory/SLInventoryComponent.h"
#include "MyPlayer.h"
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDClass)
	{
		PlayerHUDInstance = CreateWidget<USLPlayerHUD>(this, PlayerHUDClass);
		if (PlayerHUDInstance)
		{
			PlayerHUDInstance->AddToViewport();
		}
	}

	AMyPlayer* MyPlayer = Cast<AMyPlayer>(GetPawn());
	// 캐릭터에서 InventoryComponent를 가져옵니다.
	if (PlayerHUDInstance && MyPlayer)
	{
		if (USLInventoryComponent* InventoryComponent = Cast<USLInventoryComponent>(MyPlayer->GetComponentByClass(USLInventoryComponent::StaticClass())))
		{
			// 컴포넌트의 델리게이트에 UI 갱신 함수 바인딩
			InventoryComponent->OnInventoryUpdated.AddDynamic(PlayerHUDInstance, &USLPlayerHUD::UpdateItemSlot);
			InventoryComponent->AddInventorySlot.AddDynamic(PlayerHUDInstance, &USLPlayerHUD::AddItemSlot);
			InventoryComponent->OnItemAcquired.AddDynamic(PlayerHUDInstance, &USLPlayerHUD::ShowItemInfo);
			InventoryComponent->OnQuickItemUpdated.AddDynamic(PlayerHUDInstance, &USLPlayerHUD::UpdateQuickItem);

			if (PlayerHUDInstance)
				PlayerHUDInstance->SetInventoryComponent(InventoryComponent);

			InventoryComponent->InitInventory(100);
			for (int Index = 1; Index < 10; ++Index)
			{
				FString NumberString = FString::Printf(TEXT("%03d"), Index);
				FName ItemID = FName(*("Item_" + NumberString));
				InventoryComponent->AddItem(ItemID, 1);
			}
		}
	}


}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CurrentCharacterContext, 0);
		}
	}
}

void AMyPlayerController::HiddenItemText()
{
	if (PlayerHUDInstance)
	{
		PlayerHUDInstance->HiddenItemGetText();
	}
}

void AMyPlayerController::ShowItemText()
{
	if (PlayerHUDInstance) 
	{
		PlayerHUDInstance->ShowItemGetText();
	}
}

void AMyPlayerController::EquipmentWeapon()
{
	if (PlayerHUDInstance)
	{
		PlayerHUDInstance->EquipWeapon();
	}
}

void AMyPlayerController::UnequipmentWeapon()
{
	if (PlayerHUDInstance)
	{
		PlayerHUDInstance->UnequipWeapon();
	}
}

void AMyPlayerController::ToggleInventory()
{
	if (PlayerHUDInstance) 
	{
		if (PlayerHUDInstance->ToggleInventory())
		{
			// 게임 월드와 상호작용도 필요
			FInputModeGameAndUI InputMode;
			// 포커스 설정
			InputMode.SetWidgetToFocus(PlayerHUDInstance->TakeWidget());

			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
		else
		{
			//마우스 게임모드 및 감추기
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
		}
	}

}