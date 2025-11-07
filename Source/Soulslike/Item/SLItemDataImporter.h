// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemTableRow.h"

/**
 * 
 */
class SOULSLIKE_API SLItemDataImporter
{
public:
	SLItemDataImporter();
	~SLItemDataImporter();
public:
	static void ImportAllData();
private:
	static bool LoadItemTable(TMap<FName, FItemTableRow>& OutItemData);
	static void CreateOrUpdateDataAssets();
};
