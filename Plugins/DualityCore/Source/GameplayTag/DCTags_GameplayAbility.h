// Copyright (C) 2023 owoDra

#pragma once

#include "NativeGameplayTags.h"


///////////////////////////////////////////////////////
// Ability.ActivateFail

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_ActivateFail_Cooldown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_ActivateFail_TagsBlocked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_ActivateFail_TagsMissing);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_ActivateFail_Cost);


///////////////////////////////////////////////////////
// Ability.Type

DUALITYCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Type_Movement);
DUALITYCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Type_Action);
DUALITYCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Type_Passive);
DUALITYCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Type_Skill);
DUALITYCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Type_StatusChange);
DUALITYCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Type_Misc);
