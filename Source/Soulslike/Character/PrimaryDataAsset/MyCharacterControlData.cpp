// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PrimaryDataAsset/MyCharacterControlData.h"

UMyCharacterControlData::UMyCharacterControlData()
{
	// TPS 뷰

	bOrientRotationToMovement = true;
	RotationRate.Yaw = 720;

	TargetArmLength = 400.0f;

	bUsePawnControlRotation = true;
	bInheritPitch = true;
	bInheritYaw = true;
	bInheritRoll = true;
	bDoCollisionTest = true;
}
