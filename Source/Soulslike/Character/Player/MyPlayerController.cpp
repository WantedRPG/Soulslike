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

	// 캐릭터에서 InventoryComponent를 가져옵니다.
	if (AMyPlayer* MyPlayer = Cast<AMyPlayer>(GetPawn()))
	{
		if (USLInventoryComponent* InventoryComponent = Cast<USLInventoryComponent>(MyPlayer->GetComponentByClass(USLInventoryComponent::StaticClass())))
		{
			// 컴포넌트의 델리게이트에 UI 갱신 함수 바인딩
			InventoryComponent->OnInventoryUpdated.AddDynamic(this, &AMyPlayerController::UpdateInventoryUI);
			InventoryComponent->AddInventorySlot.AddDynamic(this, &AMyPlayerController::AddInventorySlotUI);

			if (PlayerHUDInstance)
				PlayerHUDInstance->SetInventoryComponent(InventoryComponent);

			InventoryComponent->InitInventory(100);
			//Todo : 델리게이트 이용해서 할것 : 아이템슬롯생성
			InventoryComponent->AddItem(FName(TEXT("Item_002")), 999);
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


void AMyPlayerController::ToggleInventory()
{
	if (PlayerHUDInstance) // CurrentHUD는 UMyHUDWidget* 타입이어야 함
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

void AMyPlayerController::UpdateInventoryUI(const FInventorySlotData& ChangedItemInfo)
{
	// 여기서 HUD 위젯 인스턴스의 함수를 호출하여 텍스트나 슬롯을 갱신합니다.
	if (PlayerHUDInstance)
	{
		PlayerHUDInstance->UpdateItemSlot(ChangedItemInfo);
	}
}

void AMyPlayerController::AddInventorySlotUI(int32 LastIndex, int32 InAddSlotCount)
{
	if (PlayerHUDInstance)
	{
		PlayerHUDInstance->AddItemSlot(LastIndex, InAddSlotCount);
	}
}
