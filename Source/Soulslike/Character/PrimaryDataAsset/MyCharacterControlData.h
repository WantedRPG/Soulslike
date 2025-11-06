// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UMyCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UMyCharacterControlData();

	// Pawn
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;					// 컨트롤러의 Yaw(좌우회전)을 따를지 여부

#pragma region 캐릭터 움직임
	// 이동 벡터 방향으로 회전. (wasd. 컨트롤러 방향 아님.)
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	// 컨트롤러 방향으로 회전. (마우스 방향)
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	// 회전 민첩성
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;
#pragma endregion								

#pragma region 카메라
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	// 캐릭터 회전 값을 따를지 여부
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	// 충돌 시 자동으로 SpringArm 길이를 조절해 끼임 방지
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
#pragma endregion	

};
