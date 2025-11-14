// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SLItemManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USLItemManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	class USLItemData* GetItemData(FName ItemID) const;
	FORCEINLINE TMap<FName, TObjectPtr<USLItemData>>& GetAllItemData() { return ItemDataCache; };
private:
	void LoadAllItemData();
	void GetCsvFilePath(FString& OutFilePath, const TCHAR* Settings, const TCHAR* CsvName) const;
	UDataTable* LoadDataTable(const TCHAR* Settings, const TCHAR* CsvName,UScriptStruct* BaseStruct) const;
private:
	UPROPERTY()
	TMap<FName, TObjectPtr<USLItemData>> ItemDataCache;
};
