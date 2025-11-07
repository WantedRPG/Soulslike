// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameplayAbilityTargetActor/MyTA_Target.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "AttributeSet/SLAttributeSet.h"

void AMyTA_Target::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	// 공격 주체(플레이어) 저장
	SourceActor = Ability ? Ability->GetCurrentActorInfo()->AvatarActor.Get() : nullptr;
}

void AMyTA_Target::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		const FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AMyTA_Target::MakeTargetData() const
{
	// Player 정보 셋팅
	ACharacter* Character = Cast<ACharacter>(SourceActor);

	if (!Character || !Character->GetCapsuleComponent())
	{
		return FGameplayAbilityTargetDataHandle();
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const USLAttributeSet* AttributeSet = ASC->GetSet<USLAttributeSet>();
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("ABCharacterAttributeSet not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	// 피격 결과
	FHitResult OutHitResult;

	// 피격 범위 설정
	const float AttackRange = 100.0f; 
	const float AttackRadius = 50.0f; 
	const float CapsuleRadius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * CapsuleRadius;
	const FVector End = Start + Forward * AttackRange;

	// 충돌 파라미터
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ATA_AI_Hit), false, Character);

	// 구체 트레이스로 Pawn 채널 대상으로 피격 판정
	const bool bHit = Character->GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_Pawn, // TODO. 충돌 채널 변경하기
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 피격 대상 정보 저장
	FGameplayAbilityTargetDataHandle DataHandle;
	if (bHit)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

	// 디버그 캡슐 표시
#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		const FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		const float  CapsuleHalfHeight = AttackRange * 0.5f;
		const FColor DrawColor = bHit ? FColor::Green : FColor::Red;

		DrawDebugCapsule(
			GetWorld(),
			CapsuleOrigin,
			CapsuleHalfHeight,
			AttackRadius,
			FRotationMatrix::MakeFromZ(Forward).ToQuat(),
			DrawColor,
			false,        
			5.0f          
		);
	}
#endif

	// 타겟 데이터 반환
	return DataHandle;
}
