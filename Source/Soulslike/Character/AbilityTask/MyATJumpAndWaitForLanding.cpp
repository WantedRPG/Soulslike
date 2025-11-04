// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AbilityTask/MyATJumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UMyATJumpAndWaitForLanding::UMyATJumpAndWaitForLanding()
{
}

UMyATJumpAndWaitForLanding* UMyATJumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UMyATJumpAndWaitForLanding* NewTask = NewAbilityTask<UMyATJumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UMyATJumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	// 착지 이벤트 바인딩
	Character->LandedDelegate.AddDynamic(this, &UMyATJumpAndWaitForLanding::OnLandedCallback);
	// 점프
	Character->Jump();
	// 아바타에서 착지 이벤트 대기
	SetWaitingOnAvatar();
}

void UMyATJumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	// 바인딩 해제
	Character->LandedDelegate.RemoveDynamic(this, &UMyATJumpAndWaitForLanding::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

void UMyATJumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}