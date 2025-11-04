// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MyCharacterControlData.h"

AMyCharacter::AMyCharacter()
{
    // Rotation
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterCapsule"));

    // Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -100), FRotator(0, -90, 0));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

    // Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;     
    GetCharacterMovement()->RotationRate = FRotator(0, 500, 0);  
    GetCharacterMovement()->JumpZVelocity = 700.f;               
    GetCharacterMovement()->AirControl = 0.35f;                  
    GetCharacterMovement()->MaxWalkSpeed = 500.f;                
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;           
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;  
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCharacter::SetCharacterControlData(const UMyCharacterControlData* CharacterControlData)
{
    // Pawn
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    // CharacterMovement
    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

