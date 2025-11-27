// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemManagerSubsystem.h"
#include "Soulslike.h" 
#include "SLItemData.h" 
#include "HAL/FileManager.h"
#include "Misc/ConfigCacheIni.h" 
void USLItemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadAllItemData();
}

void USLItemManagerSubsystem::Deinitialize()
{
	ItemDataCache.Empty();
	Super::Deinitialize();
}

USLItemData* USLItemManagerSubsystem::GetItemData(FName ItemID) const
{
	if (!ItemID.IsNone()&&ItemDataCache.Contains(ItemID))
	{
		return ItemDataCache[ItemID];
	}
	return nullptr;
}



void USLItemManagerSubsystem::LoadAllItemData()
{
	//데이터 테이블 첫번째 컬럼은 중복값이 들어갈 수 없음 -> 출력이 안됨
	UDataTable* ItemTable = LoadDataTable(TEXT("CsvPathSettings"),TEXT("ITEM_TABLE_FILE"), FItemTableRow::StaticStruct());
	UDataTable* ActionTable = LoadDataTable(TEXT("CsvPathSettings"), TEXT("ACTION_TABLE_FILE"), FActionTableRow::StaticStruct());
	UDataTable* ItemToActionTable = LoadDataTable(TEXT("CsvPathSettings"), TEXT("ITEM_TO_ACTION_TABLE_FILE"), FItemToActionTableRow::StaticStruct());
	UDataTable* EffectTable = LoadDataTable(TEXT("CsvPathSettings"), TEXT("EFFECT_TABLE_FILE"), FEffectTableRow::StaticStruct());
	UDataTable* ItemToEffectTable = LoadDataTable(TEXT("CsvPathSettings"), TEXT("ITEM_TO_EFFECT_TABLE_FILE"), FItemToEffectTableRow::StaticStruct());
	UDataTable* TextKoTable = LoadDataTable(TEXT("CsvPathSettings"), TEXT("TEXT_KO_TABLE_FILE"), FItemTextTableRow::StaticStruct());
	UDataTable* ItemTagTable = LoadDataTable(TEXT("CsvPathSettings"), TEXT("ITEM_TAG_TABLE_FILE"), FItemTagTableRow::StaticStruct());

	if (!ItemTable || !ActionTable || !ItemToActionTable || !EffectTable || !ItemToEffectTable || !TextKoTable || !ItemTagTable)
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 데이터 로딩에 실패하여 게임을 시작할 수 없습니다."));
		return;
	}

	TMap<FName, FItemTextTableRow> TempTextKoDataCache;
	TextKoTable->ForeachRow<FItemTextTableRow>(TEXT("ItemToActionTable"), [&](const FName& ItemID, const FItemTextTableRow& Row)
		{
			TempTextKoDataCache.Add(ItemID, Row);
		}
	);
	ItemTable->ForeachRow<FItemTableRow>(TEXT("ItemTable"), [&](const FName& ItemID, const FItemTableRow& Row)
		{
			USLItemData* NewItemData = NewObject<USLItemData>(this);
			NewItemData->ItemID = ItemID;
			NewItemData->ItemType = Row.ItemType;
			NewItemData->ItemName = TempTextKoDataCache[Row.TextID].ItemName;
			NewItemData->ItemDescription = TempTextKoDataCache[Row.TextID].ItemDesc;
			NewItemData->MaxStack = Row.MaxStack;
			NewItemData->ItemIcon = Row.ItemIcon;

			
			ItemDataCache.Add(ItemID, NewItemData);
		}
	);
	if (ItemDataCache.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataCache가 비어있어서 실패했습니다."));
		return;
	}

	ItemToActionTable->ForeachRow<FItemToActionTableRow>(TEXT("ItemToActionTable"), [&](const FName& TableNum, const FItemToActionTableRow& Row)
		{
			FActionTableRow* ActionRow = ActionTable->FindRow<FActionTableRow>(Row.ActionID, "FActionTableRow");
			if (ActionRow)
			{
				FItemActionDetail ItemActionDetail;
				ItemActionDetail.ActionTag = ActionRow->ActionTag;

				ItemDataCache[Row.ItemID]->ItemActionMap.Add(Row.ActionType, MoveTemp(ItemActionDetail));
			}
		}
	);
		
	ItemToEffectTable->ForeachRow<FItemToEffectTableRow>(TEXT("ItemToEffectTable"), [&](const FName& TableNum, const FItemToEffectTableRow& Row)
		{
			
			UE_LOG(LogTemp, Warning, TEXT("FItemToEffectTableRow %d %s %s"), Row.ActionType, *Row.ItemID.ToString(), *Row.EffectID.ToString());
			
			FEffectTableRow* EffectRow = EffectTable->FindRow<FEffectTableRow>(Row.EffectID, "FEffectTableRow");
			if (EffectRow)
			{
				const TObjectPtr<USLItemData>* FoundItemDataPtr = ItemDataCache.Find(Row.ItemID);
				if (FoundItemDataPtr && FoundItemDataPtr->Get()->ItemActionMap.Find(Row.ActionType))
				{
					FoundItemDataPtr->Get()->ItemActionMap[Row.ActionType].AbilityToEffects.Add (*EffectRow);
				}
			}
		}
	);

	ItemTagTable->ForeachRow<FItemTagTableRow>(TEXT("FItemTagTable"), [&](const FName& TableNum, const FItemTagTableRow& Row)
		{

			ItemDataCache[Row.ItemID]->ItemTags.AddTag(Row.ItemTag);
		}
	);
}

void USLItemManagerSubsystem::GetCsvFilePath(FString& OutFilePath, const TCHAR* Settings, const TCHAR* CsvName) const
{
	GConfig->GetString(Settings, CsvName, OutFilePath, GGameIni);
	OutFilePath = FPaths::ProjectContentDir() / OutFilePath;
}

UDataTable* USLItemManagerSubsystem::LoadDataTable(const TCHAR* Settings, const TCHAR* CsvName, UScriptStruct* BaseStruct) const
{
	FString CSVFilePath;
	GetCsvFilePath(CSVFilePath, Settings, CsvName);

	FString FileContents;
	if (!FFileHelper::LoadFileToString(FileContents, *CSVFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file at: %s"), *CSVFilePath);
		return nullptr;
	}

	UDataTable* DataTable = NewObject<UDataTable>();
	DataTable->RowStruct = BaseStruct;
	DataTable->CreateTableFromCSVString(FileContents);
	return DataTable;
}
