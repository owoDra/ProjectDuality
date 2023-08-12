
// Copyright (C) 2023 owoDra

#include "DCInputComponent.h"

#include "Player/DCLocalPlayer.h"
#include "GameSetting/DCGameSettings.h"
#include "Input/DCData_InputConfig.h"

#include "EnhancedInputSubsystems.h"
#include "InputCoreTypes.h"
#include "UObject/NameTypes.h"
#include "UObject/UnrealNames.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCInputComponent)


void UDCInputComponent::AddInputMappings(const UDCData_InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	UDCLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<UDCLocalPlayer>();
	check(LocalPlayer);

	// Add any registered input mappings from the settings!
	if (auto* Settings = UDCGameSettings::Get())
	{
		// Tell enhanced input about any custom keymappings that the player may have customized
		for (const TPair<FName, FKey>& Pair : Settings->GetCustomPlayerInputConfig())
		{
			if (Pair.Key != NAME_None && Pair.Value.IsValid())
			{
				InputSubsystem->AddPlayerMappedKey(Pair.Key, Pair.Value);
			}
		}
	}
}

void UDCInputComponent::RemoveInputMappings(const UDCData_InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	UDCLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<UDCLocalPlayer>();
	check(LocalPlayer);
	
	InputSubsystem->ClearAllMappings();
	InputSubsystem->RemoveAllPlayerMappedKeys();
}

void UDCInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}

template<class UserClass, typename FuncType>
void UDCInputComponent::BindNativeAction(const UDCData_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UDCInputComponent::BindAbilityActions(const UDCData_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const auto& Action : InputConfig->AbilityInputActions)
	{
		if (Action.Key && Action.Value.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.Key, ETriggerEvent::Started, Object, PressedFunc, Action.Value).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.Key, ETriggerEvent::Completed, Object, ReleasedFunc, Action.Value).GetHandle());
			}
		}
	}
}