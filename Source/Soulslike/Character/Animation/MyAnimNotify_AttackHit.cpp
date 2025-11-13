// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/MyAnimNotify_AttackHit.h"
#include "AbilitySystemBlueprintLibrary.h"

UMyAnimNotify_AttackHit::UMyAnimNotify_AttackHit()
{
	ComboAttackLevel = 1.0f;
}

FString UMyAnimNotify_AttackHit::GetNotifyName_Implementation() const
{
	// TODO. 추후 노티파이에 태그 연결해야 함.
	return TEXT("GAS AttackHit");
}

void UMyAnimNotify_AttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			// GE의 Level 전달
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = ComboAttackLevel;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
