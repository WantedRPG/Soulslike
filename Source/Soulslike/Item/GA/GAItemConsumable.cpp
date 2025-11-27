// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/GA/GAItemConsumable.h"
#include "Item/GA/ItemDataObject.h"
#include "Item/SLItemManagerSubsystem.h"
#include <Item/SLItemData.h>
#include "AbilitySystemComponent.h"

void UGAItemConsumable::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //유효성 검사
    if (!ActorInfo->AbilitySystemComponent.IsValid())
    {
        Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    const UItemDataObject* ItemDataObject = Cast<UItemDataObject>(TriggerEventData->OptionalObject);
    if (nullptr==ItemDataObject)
    {
        Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    const USLItemManagerSubsystem* ItemManager = UGameInstance::GetSubsystem<USLItemManagerSubsystem>(GetWorld()->GetGameInstance());
    if (nullptr == ItemManager)
    {
        Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    USLItemData* ItemData = ItemManager->GetItemData(ItemDataObject->ItemID);
    if (nullptr == ItemData)
    {
        Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (nullptr == ItemBasicEffectClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemBasicEffectClass None"));
        Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    UAbilitySystemComponent* TargetASC = ActorInfo->AbilitySystemComponent.Get();

    // Context 생성 ( 누가 발동시켰는지 등의 정보를 담음 )
    FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();

    FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(ItemBasicEffectClass, 1.0f, EffectContext);

    FGameplayTag HealMagnitudeTag = FGameplayTag::RequestGameplayTag(FName("Item.Effect.Health"));
    FGameplayTag MaxHealthMagnitudeTag = FGameplayTag::RequestGameplayTag(FName("Item.Effect.MaxHealth"));
    FGameplayTag MovementSpeedMagnitudeTag = FGameplayTag::RequestGameplayTag(FName("Item.Effect.MovementSpeed"));
    FGameplayTag MaxMovementSpeedMagnitudeTag = FGameplayTag::RequestGameplayTag(FName("Item.Effect.MaxMovementSpeed"));
    FGameplayTag AttackPowerTag = FGameplayTag::RequestGameplayTag(FName("Item.Effect.AttackPower"));
    FGameplayTag AttackRateTag = FGameplayTag::RequestGameplayTag(FName("Item.Effect.AttackRate"));
    FGameplayTag StaminaTag = FGameplayTag::RequestGameplayTag(FName("Item.Effect.Stamina"));
    FGameplayTag MaxStaminaTag = FGameplayTag::RequestGameplayTag(FName("Item.Effect.MaxStamina"));

    if (EffectContext.IsValid())
    {

        for (auto& GEInfo : ItemData->ItemActionMap[EItemActionType::Primary].AbilityToEffects)
        {
            SpecHandle.Data->SetSetByCallerMagnitude(HealMagnitudeTag, 0.0f);
            SpecHandle.Data->SetSetByCallerMagnitude(MaxHealthMagnitudeTag, 0.0f);
            SpecHandle.Data->SetSetByCallerMagnitude(MovementSpeedMagnitudeTag, 0.0f);
            SpecHandle.Data->SetSetByCallerMagnitude(MaxMovementSpeedMagnitudeTag, 0.0f);
            SpecHandle.Data->SetSetByCallerMagnitude(AttackPowerTag, 0.0f);
            SpecHandle.Data->SetSetByCallerMagnitude(AttackRateTag, 0.0f);
            SpecHandle.Data->SetSetByCallerMagnitude(StaminaTag, 0.0f);
            SpecHandle.Data->SetSetByCallerMagnitude(MaxStaminaTag, 0.0f);

            SpecHandle.Data->SetSetByCallerMagnitude(GEInfo.EffectTag, GEInfo.Value);
            FActiveGameplayEffectHandle ActiveGEHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

            


            if (ActiveGEHandle.IsValid())
            {
                UE_LOG(LogTemp, Warning, TEXT("GE Completion %d"), ItemData->ItemActionMap[EItemActionType::Primary].AbilityToEffects.Num());
            }
        }
    }
    
   
    //어빌리티 종료
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UGAItemConsumable::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UGAItemConsumable::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
}
