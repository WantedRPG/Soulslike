// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/MyAnimNotify_WeaponHit.h"
#include "AbilitySystemBlueprintLibrary.h"

FString UMyAnimNotify_WeaponHit::GetNotifyName_Implementation() const
{
	// TODO. 추후 노티파이에 태그 연결해야 함.
	return TEXT("GAS WeaponHit");
}

void UMyAnimNotify_WeaponHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			// TODO. 
			// PayloadData.EventMagnitude = ;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}