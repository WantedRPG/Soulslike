
#include "MyTA_Sword.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

void AMyTA_Sword::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability ? Ability->GetCurrentActorInfo()->AvatarActor.Get() : nullptr;
}

void AMyTA_Sword::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		const FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AMyTA_Sword::MakeTargetData() const
{
	FGameplayAbilityTargetDataHandle Out;

	ACharacter* Character = Cast<ACharacter>(SourceActor);
	if (!Character) return Out;

	UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
	if (!Capsule) return Out;

	// 캐릭터 앞 기준으로 박스 중심 계산
	const FVector Up = Character->GetActorUpVector();
	const FVector Fwd = Character->GetActorForwardVector();

	const float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	const float Radius = Capsule->GetScaledCapsuleRadius();

	const FVector CapsuleCenter = Character->GetActorLocation();
	const FVector CapsuleBottom = CapsuleCenter - Up * HalfHeight;

	FVector BoxHalfExtent = FVector(40.f, 40.f, 30.f);
	float GroundOffset = 5.f;
	float ForwardOffset = 30.f;

	const FVector BoxCenter = CapsuleBottom + Up * (BoxHalfExtent.Z + GroundOffset) + Fwd * (Radius + ForwardOffset);

	// 충돌 파라미터
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ATA_FrontFootBox_Overlap), false, Character);
	// 캐릭터는 무시
	Params.AddIgnoredActor(Character);

	TArray<FHitResult> Hits;
	Character->GetWorld()->SweepMultiByChannel(
		Hits,
		BoxCenter,
		BoxCenter,
		FQuat::Identity, 
		TraceChannel,
		FCollisionShape::MakeBox(BoxHalfExtent),
		Params
	);

	// TODO. 추후 로직 수정 예정. 무기 후보 중 하나 선택
	bool bFound = false;
	FHitResult OutHitResult;
	float BestDistSq = TNumericLimits<float>::Max();
	const FVector BoxOrigin = Character->GetActorLocation();

	for (const FHitResult& HR : Hits)
	{
		AActor* A = HR.GetActor();
		if (!A) 
		{
			continue;
		}

		// 태그 필터
		if (RequiredActorTag != NAME_None && !A->ActorHasTag(RequiredActorTag))
		{
			continue;
		}

		const float DistSq = FVector::DistSquared(BoxOrigin, A->GetActorLocation());
		if (!bFound || DistSq < BestDistSq)
		{
			bFound = true;
			BestDistSq = DistSq;
			OutHitResult = HR;
		}
	}

	if (bFound)
	{
		Out.Add(new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult));
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		DrawDebugBox(Character->GetWorld(), BoxCenter, BoxHalfExtent, FQuat::Identity, bFound ? FColor::Green : FColor::Red, false, 3.f, 0, 1.f);

		if (bFound)
		{
			const FVector P = OutHitResult.ImpactPoint.IsZero() ? OutHitResult.Location : OutHitResult.ImpactPoint;
			DrawDebugLine(Character->GetWorld(), BoxOrigin, P, FColor::Green, false, 3.f, 0, 1.f);
		}
	}
#endif

	return Out;
}


