// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "AbilitySystemInterface.h"
#include "Character/PrimaryDataAsset/MyPDAComboAttack.h"
#include "MyPlayer.generated.h"

class UGameplayAbility;
class UInputAction;

/**
 * 
 */
UCLASS()
class SOULSLIKE_API AMyPlayer : public AMyCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMyPlayer();

public:
	FORCEINLINE UAnimMontage* GetComboActionMontage() const { return ComboAttackMontage; }
	FORCEINLINE UMyPDAComboAttack* GetComboActionData() const { return ComboAttackData; }

protected:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void SetupGASInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

protected:
	void Move(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);
	void EquipWeapon(const FInputActionValue& Value);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<TSubclassOf<class UGameplayEffect>, int32> StatEffect;

	// UPROPERTY(EditAnywhere, Category = GAS)
	// float Level;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Move", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Move", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Move", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Move", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Move", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EquipWeaponAction;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ComboAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FlameAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ElectricityAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AcidAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ImpactAction;*/

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Weapon", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> SwordMesh;

	bool bHasSwordEquipped = false;

protected:
	UPROPERTY(EditAnywhere, Category = "Attack | Melee")
	UAnimMontage* ComboAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack | Melee")
	TObjectPtr<class UMyPDAComboAttack> ComboAttackData;
};
