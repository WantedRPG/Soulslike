// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/MyCharacterControlData.h"

AMyCharacter::AMyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Rotation
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Capsule
    // GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterCapsule"));

    // Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
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

    // Camera
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.f;
    CameraBoom->bUsePawnControlRotation = true;
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (CharacterControlDataAsset)
    {
        SetCharacterControlData(CharacterControlDataAsset);
        return;
    }
}

void AMyCharacter::SetCharacterControlData(const UMyCharacterControlData* CharacterControlData)
{
    // Pawn
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    // CharacterMovement
    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

    // CameraBoom
    CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
    CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
    CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
    CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
    CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
    CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
    CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

