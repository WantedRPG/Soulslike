// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MyCharacter.generated.h"

class UMyCharacterControlData;

UCLASS()
class SOULSLIKE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetCharacterControlData(const class UMyCharacterControlData* CharacterControlData);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMyCharacterControlData> CharacterControlDataAsset;
};
