#pragma once

#include "GameplayTagContainer.h"

// 무적 상태 태그
#define MyTAG_Invincibility			 FGameplayTag::RequestGameplayTag(FName("Character.State.Roll"))

// Hp바 무적 상태 태그
#define MyTag_INVINSIBLE			 FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))