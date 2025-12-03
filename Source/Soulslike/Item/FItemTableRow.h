#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SLItemEums.h"
#include "Abilities/GameplayAbility.h"
#include "FItemTableRow.generated.h"


class UGameplayEffect;
class UGameplayAbility;

/*
 * 아이템 정보를 정의하는 데이터 테이블 Row 구조체
 * 게임 내 모든 아이템의 기본 속성을 관리하며, 다른 아이템 관련 테이블의 시작점 역할
 */
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

/*
 * 게임 내 아이템 텍스트 정보를 정의하는 데이터 테이블 Row 구조체
 * 다국어 지원을 위해 텍스트를 중앙에서 관리하며, ItemID와 TextID를 분리하여 데이터 정규화를 수행
 */
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

/*
 * 아이템과 해당 아이템의 행동을 연결하는 데이터 테이블 Row 구조체
 * 특정 아이템이 어떤 행동 타입(ActionType: 주 사용, 해제 시)일 때, 어떤 액션(ActionID)을 수행하는지를 정의
 */
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

/*
 * 게임 내 행동(Action)의 상세 정의를 담는 데이터 테이블 Row 구조체
 * 특정 행동이 어떤 GameplayAbility를 발동시키고 어떤 GameplayTag를 가지는지 정의
 */
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

/*
 * 게임 내 효과(Effect)의 상세 정의를 담는 데이터 테이블 Row 구조체
 * 하나의 범용 GameplayEffect가 데이터에 따라 다양한 효과를 낼 수 있도록 파라미터를 정의
 */
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

/*
 * 아이템과 해당 아이템이 가지는 효과를 직접 연결하는 데이터 테이블 Row 구조체
 * FItemTableRow와 FEffectTableRow 간의 연결 테이블 역할
 */
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

/*
 * USLItemData 내부에서 아이템의 '행동'과 그에 따른 '효과'를 통합하는 구조체
 * FItemToActionTableRow와 FActionTableRow, FEffectTableRow의 정보를 최종적으로 조합
 * 런타임에 아이템의 행동 및 효과 정보를 효율적으로 접근할 수 있도록 캐싱하는 데 사용
 */
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

/*
 * 아이템에 GameplayTag를 부여하여 추가적인 속성을 정의하는 데이터 테이블 Row 구조체
 * 아이템의 속성을 관리하고, 시스템 간의 상호작용을 위한 태그 정보를 제공
 */
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
