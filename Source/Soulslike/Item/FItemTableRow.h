#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SLItemEums.h"
#include "Abilities/GameplayAbility.h"
#include "FItemTableRow.generated.h"


class UGameplayEffect;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TextID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> ItemIcon;

	FItemTableRow() : TextID(NAME_None) ,MaxStack(0),ItemType(EItemType::None) {}
};

USTRUCT(BlueprintType)
struct FItemTextTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TextID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDesc;

	FItemTextTableRow() : ItemName(), ItemDesc() {}
};

USTRUCT(BlueprintType)
struct FItemToActionTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TableNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemActionType ActionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActionID;

	FItemToActionTableRow() : TableNum(),ItemID(), ActionType(EItemActionType::None), ActionID() {}
};

USTRUCT(BlueprintType)
struct FActionTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActionID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	TSoftClassPtr<UGameplayAbility> GameplayAbilityPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	FGameplayTag ActionTag;
};

USTRUCT(BlueprintType)
struct FEffectTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UGameplayEffect> GameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EffectTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag DurationTag;
	FEffectTableRow() : Value(0), Duration(0.0) {};
};

USTRUCT(BlueprintType)
struct FItemToEffectTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemActionType ActionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EffectID;
	FItemToEffectTableRow() : ItemID(), ActionType(EItemActionType::None), EffectID() {};
};

USTRUCT(BlueprintType)
struct FItemActionDetail : public FTableRowBase
{
	GENERATED_BODY()
public:
	//아이템 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	FGameplayTag ActionTag;
	//아이템 효과 - 여러 효과 적용 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TArray<FEffectTableRow> AbilityToEffects;
};

USTRUCT(BlueprintType)
struct FItemTagTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag;
	FItemTagTableRow() {};
};
