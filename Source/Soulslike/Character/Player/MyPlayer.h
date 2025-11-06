// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "AbilitySystemInterface.h"
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

	void SprintStart(const FInputActionValue& Value);
	void SprintStop(const FInputActionValue& Value);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RollAction;
	
};
