// Copyright (C) 2023 owoDra

#include "DCLocalPlayer.h"

#include "GameSetting/DCGameSettings.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Delegates/Delegate.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Templates/Casts.h"
#include "UObject/NameTypes.h"
#include "UObject/ObjectPtr.h"
#include "UObject/WeakObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCLocalPlayer)


void UDCLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();

	if (auto* DeviceSettings{ UDCGameSettings::Get() })
	{
		DeviceSettings->OnAudioOutputDeviceChanged.AddUObject(this, &UDCLocalPlayer::OnAudioOutputDeviceChanged);
	}
}

void UDCLocalPlayer::OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId)
{
	FOnCompletedDeviceSwap DevicesSwappedCallback;
	DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedAudioDeviceSwap"));
	UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GetWorld(), InAudioOutputDeviceId, DevicesSwappedCallback);
}

void UDCLocalPlayer::OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult)
{
}
