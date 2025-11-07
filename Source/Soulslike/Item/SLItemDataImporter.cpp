// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemDataImporter.h"
#include "Soulslike.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Factories/DataTableFactory.h" 
#include "AssetRegistry/AssetRegistryModule.h"
#include "SLItemData.h"

SLItemDataImporter::SLItemDataImporter()
{
}

SLItemDataImporter::~SLItemDataImporter()
{
}

void SLItemDataImporter::ImportAllData()
{
}
bool SLItemDataImporter::LoadItemTable(TMap<FName, FItemTableRow>& OutItemData)
{
	return false;
}

/*
bool SLItemDataImporter::LoadItemTable(TMap<FName, FItemTableRow>& OutItemData)
{
	OutItemData.Empty();
	if (!IFileManager::Get().FileExists(*ITEM_TABLE_FILE))
	{
		UE_LOG(LogTemp, Error, TEXT("DT_Item.csv 파일을 찾을 수 없습니다: %s"), *ITEM_TABLE_FILE);
		return false;
	}
	const FString TempDataTableName = TEXT("Temp_DT_Item");
	const FString TempPackagePath = TEXT("/Game/Temp");
	const FString FullTempAssetPath = TempPackagePath / TempDataTableName;
	UDataTable* DataTable = nullptr;
	DataTable = FindObject<UDataTable>(nullptr, *FullTempAssetPath);
	if (!DataTable)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath(FullTempAssetPath));
		if (AssetData.IsValid())
		{
			DataTable = Cast<UDataTable>(AssetData.GetAsset());
		}
	}
	
	if (!DataTable)
	{
		UPackage* TempPackage = CreatePackage(*TempPackagePath);
		UDataTableFactory* DataTableFactory = NewObject<UDataTableFactory>();
		DataTableFactory->Struct = FItemTableRow::StaticStruct();
		bool Result;
		DataTable = Cast<UDataTable>(DataTableFactory->FactoryCreateFile(
					 USLItemData::StaticClass(),
		             TempPackage,
		             FName(*TempDataTableName),
		             RF_Public | RF_Standalone,
					 *ITEM_TABLE_FILE,
		             nullptr,
		             GWarn,
					Result
		));

		if (!DataTable)
		{
			UE_LOG(LogTemp, Error, TEXT("DT_Item.csv 파일을 찾을 수 없습니다: %s"), *ITEM_TABLE_FILE);
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("파일을 이미 존재: %s"), *ITEM_TABLE_FILE);
	}
	DataTable->ForeachRow<FItemTableRow>(TEXT("FItemDataImporter::LoadItemTable"), [&](const FName& Key, const FItemTableRow& Value)
		{
			OutItemData.Add(Key, Value);
		});
	UE_LOG(LogTemp, Log, TEXT("DT_Item.csv 로드 및 캐싱 완료. 아이템 개수: %d"), OutItemData.Num());
	return true;
}
*/
void SLItemDataImporter::CreateOrUpdateDataAssets()
{
	TMap<FName, FItemTableRow> OutItemData;
	LoadItemTable(OutItemData);
}
