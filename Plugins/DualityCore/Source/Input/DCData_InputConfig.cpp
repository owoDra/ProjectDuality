// Copyright (C) 2023 owoDra

#include "DCData_InputConfig.h"

#include "Development/DCLogChannels.h"

#include "GameplayTagContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCData_InputConfig)


const UInputAction* UDCData_InputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (const auto* Action{ NativeInputActions.FindKey(InputTag) })
	{
		return *Action;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogDC, Error, TEXT("InputTag [%s] に関連する NativeInput から InputConfig [%s] を見つけることができませんでした。"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UDCData_InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (const auto* Action{ AbilityInputActions.FindKey(InputTag) })
	{
		return *Action;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogDC, Error, TEXT("InputTag [%s] に関連する AbilityInput から InputConfig [%s] を見つけることができませんでした。"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
