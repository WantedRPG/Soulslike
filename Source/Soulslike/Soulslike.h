// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

const FString CSV_BASE_PATH = FPaths::ProjectDir() / TEXT("ItemData");
const FString ITEM_TABLE_FILE = CSV_BASE_PATH / TEXT("DT_Item.csv");
const FString ACTION_TABLE_FILE = CSV_BASE_PATH / TEXT("DT_Action.csv");
const FString ITEM_TO_ACTION_TABLE_FILE = CSV_BASE_PATH / TEXT("DT_ItemToAction.csv");
const FString EFFECT_TABLE_FILE = CSV_BASE_PATH / TEXT("DT_Effect.csv");
const FString ITEM_TO_EFFECT_TABLE_FILE = CSV_BASE_PATH / TEXT("DT_ItemToEffect.csv");
const FString TEXT_KO_TABLE_FILE = CSV_BASE_PATH / TEXT("DT_Text_KO.csv");