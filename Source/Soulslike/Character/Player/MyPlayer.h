// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "AbilitySystemInterface.h"
#include "Character/PrimaryDataAsset/MyPDAComboAttack.h"
#include "GameplayEffectTypes.h"
#include "MyPlayer.generated.h"

class UGameplayAbility;
class UInputAction;

USTRUCT(BlueprintType)
struct FInputAbilities
{
	GENERATED_BODY()
	 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputAction> IA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayAbility> InputGA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InputId = 0;
};                                                                                                       

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Peace	  UMETA(DisplayName = "Peace"),
	// Attack    UMETA(DisplayName = "Attack"),
	// KnockBack UMETA(DisplayName = "KnockBack"),
	Dead	  UMETA(DisplayName = "Dead"),
};

UCLASS()
class SOULSLIKE_API AMyPlayer : public AMyCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMyPlayer();

public:
	// 상태 설정
	FORCEINLINE void SetPlayerMode(EPlayerState State) { PlayerMode = State; }
	FORCEINLINE EPlayerState GetPlayerMode() const { return PlayerMode; }
	FORCEINLINE bool IsDead() const { return PlayerMode == EPlayerState::Dead; }

	// 몽타주 반환	
	FORCEINLINE UAnimMontage* GetRollActionMontage() const { return RollMontage; }
	FORCEINLINE UAnimMontage* GetComboActionMontage() const { return ComboAttackMontage; }
	FORCEINLINE UAnimMontage* GetSkillActionMontage() const { return SkillAttackMontage; }
	FORCEINLINE UAnimMontage* GetSTakeHitMontage() const { return TakeHitMontage; }

	// 콤보 액션 데이터 반환
	FORCEINLINE UMyPDAComboAttack* GetComboActionData() const { return ComboAttackData; }
	
public:
	UFUNCTION()
	void StopSprint(const FGameplayEffectContextHandle& Context);

	UFUNCTION()
	void KnockBack(const FGameplayEffectContextHandle& Context, float InMagnitude);

	UFUNCTION()
	void Death();

protected:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	// 입력 바인딩
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// GAS 입력 바인딩
	void SetupGASInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

protected:
	void Move(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);
	void ToggleInventory();

public:
	UPROPERTY()
	TObjectPtr<class USLAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	EPlayerState PlayerMode = EPlayerState::Peace;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS | Input")
	TArray<FInputAbilities> InputAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> StatEffects;

	UPROPERTY(EditAnywhere, Category = "GAS")
	float Level;

protected:
	// 입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Move", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Move", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InventoryAction;

protected:	
	// 몽타주
	UPROPERTY(EditAnywhere, Category = "Montage | Roll")
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, Category = "Montage | Combo Attack")
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage | Skill Attack")
	TObjectPtr<UAnimMontage> SkillAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage | Knock Back")
	TObjectPtr<UAnimMontage> TakeHitMontage;

protected:
	// 콤보 액션 데이터
	UPROPERTY(EditAnywhere, Category = "PDA | ComboAttack")
	TObjectPtr<UMyPDAComboAttack> ComboAttackData;

protected:
	// 인벤토리 컴포넌트	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<class USLInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, Category = "Item")
	TObjectPtr<class AActor> ItemActor;

protected:
	// UI 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMyWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMyWidgetComponent> StaminaBar;
};
