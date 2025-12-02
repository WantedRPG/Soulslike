// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/PrimaryDataAsset/MyCharacterControlData.h"

AMyCharacter::AMyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Rotation
    // 컨트롤러의 회전을 캐릭터의 회전에 반영하지 않음.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Capsule
    // GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    // GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterCapsule"));

    // Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

    // Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;     // 움직이는 방향을 바라보도록 
    GetCharacterMovement()->RotationRate = FRotator(0, 500/*Yaw/Z*/, 0);  
    GetCharacterMovement()->JumpZVelocity = 700.f;               
    GetCharacterMovement()->AirControl = 0.35f;                  
    GetCharacterMovement()->MaxWalkSpeed = 600.f;                
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
		// 캐릭터 시점 설정. 3인칭
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

    // Root Component의 회전을 그대로 따를 것인지
    CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
    CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
    CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
    
	// 캐릭터의 회전을 따를 것인지
    CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
    // 충돌 시 끼임 방지
    CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

