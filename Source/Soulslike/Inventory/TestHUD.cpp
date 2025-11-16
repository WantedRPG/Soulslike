// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/TestHUD.h"
#include "TestHUD.h"
#include "SLInventoryWidget.h"
UTestHUD::UTestHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UTestHUD::NativeOnInitialized()
{

}

USLInventoryWidget* UTestHUD::GetInventoryWidget()
{
	return WBP_Inventory;
}
