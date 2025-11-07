// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemManagerSubsystem.h"
#include "Soulslike.h" 
#include "SLItemData.h" 
#include "HAL/FileManager.h"
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
	if (ItemDataCache.Contains(ItemID))
	{
		return ItemDataCache[ItemID];
	}
	return nullptr;
}

void USLItemManagerSubsystem::LoadAllItemData()
{
	//데이터 테이블 첫번째 컬럼은 중복값이 들어갈 수 없음 -> 출력이 안됨
	UDataTable* ItemTable = LoadDataTable(ITEM_TABLE_FILE, FItemTableRow::StaticStruct());
	UDataTable* ActionTable = LoadDataTable(ACTION_TABLE_FILE, FActionTableRow::StaticStruct());
	UDataTable* ItemToActionTable = LoadDataTable(ITEM_TO_ACTION_TABLE_FILE, FItemToActionTableRow::StaticStruct());
	UDataTable* EffectTable = LoadDataTable(EFFECT_TABLE_FILE, FEffectTableRow::StaticStruct());
	UDataTable* ItemToEffectTable = LoadDataTable(ITEM_TO_EFFECT_TABLE_FILE, FItemToEffectTableRow::StaticStruct());
	UDataTable* TextKoTable = LoadDataTable(TEXT_KO_TABLE_FILE, FItemTextTableRow::StaticStruct());

	if (!ItemTable || !ActionTable || !ItemToActionTable || !EffectTable || !ItemToEffectTable || !TextKoTable)
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
			
			/*
						FItemActionDetail ItemActionDetail;
						ItemActionDetail.AbilityToActivate = TempActionDataCache[TempItemToActionDataCache[ItemID].ActionID];
						for (auto& ItemToEffectData : TempItemToEffectDataCache)
						{
							if (ItemToEffectData.ItemID == ItemID)
							{

	
									Todo : 해당 정보 활용해서 이펙트 효과 만들고
									TempEffectDataCache[ItemToEffectData.EffectID].Duration;
									TempEffectDataCache[ItemToEffectData.EffectID].ItemTag;
									TempEffectDataCache[ItemToEffectData.EffectID].Value;
									TempEffectDataCache[ItemToEffectData.EffectID].GameplayEffectClass;
									//배열에 넣기
									ItemActionDetail.GameplayEffectClass.Add(TempEffectDataCache[ItemToEffectData.EffectID].GameplayEffectClass);

					}
			}
			//Todo : 복사 생성자-> 이동생성자로 , type에 맞춰서 넣어야함
			//NewItemData->ItemActionMap.Add(,ItemActionDetail);
			//NewItemData->ItemActionMap.Add(TempItemToActionDataCache[ItemID].ActionType,);
			*/
			
			ItemDataCache.Add(ItemID, NewItemData);
		}
	);
	if (ItemDataCache.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataCache가 비어있어서 실패했습니다."));
		return;
	}
		
	//TArray<FItemToEffectTableRow> TempItemToEffectDataCache;
	ItemToEffectTable->ForeachRow<FItemToEffectTableRow>(TEXT("ItemToEffectTable"), [&](const FName& TableNum, const FItemToEffectTableRow& Row)
		{
			FItemActionDetail ItemActionDetail;
			UE_LOG(LogTemp, Log, TEXT("EffectRow %d %s %s"), Row.ActionType, *Row.ItemID.ToString(), *Row.EffectID.ToString());
			
			ItemDataCache[Row.ItemID]->ItemActionMap.Add(Row.ActionType, ItemActionDetail);
			
			FEffectTableRow* EffectRow = EffectTable->FindRow<FEffectTableRow>(Row.EffectID, "FEffectTableRow");
			if (EffectRow)
			{
				//Todo : GameplayEffectClass 구현 후 넣기
				//ItemDataCache[ItemID]->ItemActionMap[Row.ActionType].EffectsToApply.Add(EffectRow->GameplayEffectClass.LoadSynchronous());
				//UE_LOG(LogTemp, Log, TEXT("EffectRow %d %s"), ItemDataCache[ItemID]->ItemActionMap.Num(), *ItemID.ToString());
			}

			//TempItemToEffectDataCache.Add(Row);
		}
	);

	//TMap<FName, FItemToActionTableRow> TempItemToActionDataCache;
	ItemToActionTable->ForeachRow<FItemToActionTableRow>(TEXT("ItemToActionTable"), [&](const FName& TableNum, const FItemToActionTableRow& Row)
		{
			FActionTableRow* ActionRow = ActionTable->FindRow<FActionTableRow>(Row.ActionID, "FActionTableRow");
			if (ActionRow)
			{
				//Todo : AbilityToActivate 구현 후 넣기
				//ItemDataCache[Row.ItemID]->ItemActionMap[Row.ActionType].AbilityToActivate = ActionRow->AbilityToActivate.LoadSynchronous();
				UE_LOG(LogTemp, Log, TEXT("ActionRow"));
			}
			//TempItemToActionDataCache.Add(ItemID, Row);
		}
	);
	/*
	* 지울 예정
	TMap<FName, TSoftClassPtr<UGameplayAbility>> TempActionDataCache;
	ActionTable->ForeachRow<FActionTableRow>(TEXT("ActionTable"), [&](const FName& ActionID, const FActionTableRow& Row)
		{
			TempActionDataCache.Add(ActionID, Row.AbilityToActivate);
		}
	);
	TMap<FName, FEffectTableRow> TempEffectDataCache;
	EffectTable->ForeachRow<FEffectTableRow>(TEXT("EffectTable"), [&](const FName& EffectID, const FEffectTableRow& Row)
		{
			TempEffectDataCache.Add(EffectID, Row);
		}
	);
	*/

	for (auto& s : ItemDataCache)
	{
		FName ItemID = s.Key;
		
		UE_LOG(LogTemp, Log, TEXT("%s %s %s"), *ItemID.ToString(),*s.Value->ItemName.ToString(), *s.Value->ItemName.ToString());
		
	}
}

UDataTable* USLItemManagerSubsystem::LoadDataTable(const FString& CSVFileName, UScriptStruct* BaseStruct) const
{
	const FString CSVFilePath = CSV_BASE_PATH / CSVFileName;
	if (!IFileManager::Get().FileExists(*CSVFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("%s 파일을 찾을 수 없습니다."), *CSVFileName);
		return nullptr;
	}
	FString FileContents;
	FFileHelper::LoadFileToString(FileContents, *CSVFilePath);

	UDataTable* DataTable = NewObject<UDataTable>();
	DataTable->RowStruct = BaseStruct;
	DataTable->CreateTableFromCSVString(FileContents);
	return DataTable;
}
