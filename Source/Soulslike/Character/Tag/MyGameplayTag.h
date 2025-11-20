#pragma once

#include "GameplayTagContainer.h"

// 무적 상태 태그
#define MyTAG_Invincibility			 FGameplayTag::RequestGameplayTag(FName("Character.State.Roll"))

// UI바 무적 상태 태그
#define MyTag_INVINSIBLE			 FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))

// 데미지 받은 상태 태그
#define MyTAG_DamageState			 FGameplayTag::RequestGameplayTag(FName("Character.State.Damage"))