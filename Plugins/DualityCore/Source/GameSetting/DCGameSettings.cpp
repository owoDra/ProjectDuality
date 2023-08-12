// Copyright (C) 2023 owoDra

#include "DCGameSettings.h"

#include "Player/DCLocalPlayer.h"
#include "Audio/DCAudioSettings.h"
#include "Audio/DCAudioMixEffectsSubsystem.h"
#include "Development/DCLogChannels.h"

#include "Framework/Application/SlateApplication.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "Sound/SoundClass.h"
#include "AudioDeviceManager.h"
#include "AudioDevice.h"
#include "AudioMixerBlueprintLibrary.h"
#include "SoundControlBus.h"
#include "AudioModulationStatics.h"
#include "EnhancedInputSubsystems.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(DCGameSettings)


UDCGameSettings::UDCGameSettings()
{
	SetToDefaults();
}

void UDCGameSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	bDesiredHeadphoneMode = bUseHeadphoneMode;
	SetHeadphoneModeEnabled(bUseHeadphoneMode);
}

void UDCGameSettings::BeginDestroy()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().OnApplicationActivationStateChanged().Remove(OnApplicationActivationStateChangedHandle);
	}

	Super::BeginDestroy();
}


void UDCGameSettings::ResetToCurrentSettings()
{
	Super::ResetToCurrentSettings();

	bDesiredHeadphoneMode = bUseHeadphoneMode;
}

void UDCGameSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();

	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	if (auto * ControlBusDblPtr{ ControlBusMap.Find(TEXT("Overall")) })
	{
		if (auto * ControlBusPtr{ *ControlBusDblPtr })
		{
			SetVolumeForControlBus(ControlBusPtr, OverallVolume);
		}
	}

	if (auto * ControlBusDblPtr{ ControlBusMap.Find(TEXT("Music")) })
	{
		if (auto * ControlBusPtr{ *ControlBusDblPtr })
		{
			SetVolumeForControlBus(ControlBusPtr, MusicVolume);
		}
	}

	if (auto * ControlBusDblPtr{ ControlBusMap.Find(TEXT("SoundFX")) })
	{
		if (auto * ControlBusPtr{ *ControlBusDblPtr })
		{
			SetVolumeForControlBus(ControlBusPtr, SoundFXVolume);
		}
	}

	if (bUseHeadphoneMode != bDesiredHeadphoneMode)
	{
		SetHeadphoneModeEnabled(bDesiredHeadphoneMode);
	}

	if (FApp::CanEverRender())
	{
		ApplyDisplayGamma();
	}

	OnSettingApplied.Broadcast(this);
}


void UDCGameSettings::ReapplyThingsDueToPossibleDeviceProfileChange()
{
	ApplyNonResolutionSettings();
}


#pragma region Culture

void UDCGameSettings::SetPendingCulture(const FString& NewCulture)
{
	PendingCulture = NewCulture;
	bResetToDefaultCulture = false;
}

void UDCGameSettings::OnCultureChanged()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

void UDCGameSettings::ClearPendingCulture()
{
	PendingCulture.Reset();
}

bool UDCGameSettings::IsUsingDefaultCulture() const
{
	FString Culture;
	GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni);

	return Culture.IsEmpty();
}

void UDCGameSettings::ResetToDefaultCulture()
{
	ClearPendingCulture();
	bResetToDefaultCulture = true;
}

void UDCGameSettings::ApplyCultureSettings()
{
	if (bResetToDefaultCulture)
	{
		const FCulturePtr SystemDefaultCulture{ FInternationalization::Get().GetDefaultCulture() };
		check(SystemDefaultCulture.IsValid());

		const auto CultureToApply{ SystemDefaultCulture->GetName() };
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			GConfig->RemoveKey(TEXT("Internationalization"), TEXT("Culture"), GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		bResetToDefaultCulture = false;
	}
	else if (!PendingCulture.IsEmpty())
	{
		const auto CultureToApply{ PendingCulture };
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			GConfig->SetString(TEXT("Internationalization"), TEXT("Culture"), *CultureToApply, GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		ClearPendingCulture();
	}
}

void UDCGameSettings::ResetCultureToCurrentSettings()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

#pragma endregion


#pragma region ColorBlindMode

void UDCGameSettings::SetColorBlindMode(EColorBlindMode InMode)
{
	if (ColorBlindMode != InMode)
	{
		ColorBlindMode = InMode;
		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
			(EColorVisionDeficiency)(int32)ColorBlindMode, (int32)ColorBlindStrength, true, false);
	}
}

void UDCGameSettings::SetColorBlindStrength(int32 InColorBlindStrength)
{
	InColorBlindStrength = FMath::Clamp(InColorBlindStrength, 0, 10);
	if (ColorBlindStrength != InColorBlindStrength)
	{
		ColorBlindStrength = InColorBlindStrength;
		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
			(EColorVisionDeficiency)(int32)ColorBlindMode, (int32)ColorBlindStrength, true, false);
	}
}

#pragma endregion


#pragma region Audio

void UDCGameSettings::LoadUserControlBusMix()
{
	if (GEngine)
	{
		if (const auto* World{ GEngine->GetCurrentPlayWorld() })
		{
			if (const auto* DCAudioSettings{ GetDefault<UDCAudioSettings>() })
			{
				USoundControlBus* OverallControlBus{ nullptr };
				USoundControlBus* MusicControlBus{ nullptr };
				USoundControlBus* SoundFXControlBus{ nullptr };

				ControlBusMap.Empty();

				if (auto* ObjPath{ DCAudioSettings->OverallVolumeControlBus.TryLoad() })
				{
					if (auto* SoundControlBus{ Cast<USoundControlBus>(ObjPath) })
					{
						OverallControlBus = SoundControlBus;
						ControlBusMap.Add(TEXT("Overall"), OverallControlBus);
					}
					else
					{
						ensureMsgf(SoundControlBus, TEXT("DCAudioSettings で定義されている Overall Control Bus の参照が無効です"));
					}
				}

				if (auto* ObjPath{ DCAudioSettings->MusicVolumeControlBus.TryLoad() })
				{
					if (auto* SoundControlBus{ Cast<USoundControlBus>(ObjPath) })
					{
						MusicControlBus = SoundControlBus;
						ControlBusMap.Add(TEXT("Music"), MusicControlBus);
					}
					else
					{
						ensureMsgf(SoundControlBus, TEXT("DCAudioSettings で定義されている Music Control Bus の参照が無効です"));
					}
				}

				if (auto* ObjPath{ DCAudioSettings->SoundFXVolumeControlBus.TryLoad() })
				{
					if (auto* SoundControlBus{ Cast<USoundControlBus>(ObjPath) })
					{
						SoundFXControlBus = SoundControlBus;
						ControlBusMap.Add(TEXT("SoundFX"), SoundFXControlBus);
					}
					else
					{
						ensureMsgf(SoundControlBus, TEXT("DCAudioSettings で定義されている SoundFX Control Bus の参照が無効です"));
					}
				}

				if (auto* ObjPath{ DCAudioSettings->UserSettingsControlBusMix.TryLoad() })
				{
					if (auto* SoundControlBusMix{ Cast<USoundControlBusMix>(ObjPath) })
					{
						ControlBusMix = SoundControlBusMix;

						const FSoundControlBusMixStage OverallControlBusMixStage	= UAudioModulationStatics::CreateBusMixStage(World, OverallControlBus, OverallVolume);
						const FSoundControlBusMixStage MusicControlBusMixStage		= UAudioModulationStatics::CreateBusMixStage(World, MusicControlBus, MusicVolume);
						const FSoundControlBusMixStage SoundFXControlBusMixStage	= UAudioModulationStatics::CreateBusMixStage(World, SoundFXControlBus, SoundFXVolume);

						TArray<FSoundControlBusMixStage> ControlBusMixStageArray;
						ControlBusMixStageArray.Add(OverallControlBusMixStage);
						ControlBusMixStageArray.Add(MusicControlBusMixStage);
						ControlBusMixStageArray.Add(SoundFXControlBusMixStage);

						UAudioModulationStatics::UpdateMix(World, ControlBusMix, ControlBusMixStageArray);

						bSoundControlBusMixLoaded = true;
					}
					else
					{
						ensureMsgf(SoundControlBusMix, TEXT("DCAudioSettings で定義されている User Settings Control Bus Mix の参照が無効です"));
					}
				}
			}
		}
	}
}

void UDCGameSettings::SetAudioOutputDeviceId(const FString& InAudioOutputDeviceId)
{
	AudioOutputDeviceId = InAudioOutputDeviceId;
	OnAudioOutputDeviceChanged.Broadcast(InAudioOutputDeviceId);
}

void UDCGameSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;

	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix の読み込みに失敗ました"));

	if (auto* ControlBusDblPtr{ ControlBusMap.Find(TEXT("Overall")) })
	{
		if (auto * ControlBusPtr{ *ControlBusDblPtr })
		{
			SetVolumeForControlBus(ControlBusPtr, OverallVolume);
		}
	}
}

void UDCGameSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;

	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix の読み込みに失敗ました"));

	if (auto* ControlBusDblPtr{ ControlBusMap.Find(TEXT("Music")) })
	{
		if (auto* ControlBusPtr{ *ControlBusDblPtr })
		{
			SetVolumeForControlBus(ControlBusPtr, OverallVolume);
		}
	}
}

void UDCGameSettings::SetSoundFXVolume(float InVolume)
{
	SoundFXVolume = InVolume;

	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix の読み込みに失敗ました"));

	if (auto* ControlBusDblPtr{ ControlBusMap.Find(TEXT("SoundFX")) })
	{
		if (auto* ControlBusPtr{ *ControlBusDblPtr })
		{
			SetVolumeForControlBus(ControlBusPtr, OverallVolume);
		}
	}
}

void UDCGameSettings::SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume)
{
	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix 設定の読み込みに失敗しました"));

	if (GEngine && InSoundControlBus && bSoundControlBusMixLoaded)
	{
		if (const auto * AudioWorld{ GEngine->GetCurrentPlayWorld() })
		{
			ensureMsgf(ControlBusMix, TEXT("ControlBusMix の読み込みに失敗しました"));

			FSoundControlBusMixStage UpdatedControlBusMixStage;
			UpdatedControlBusMixStage.Bus = InSoundControlBus;
			UpdatedControlBusMixStage.Value.TargetValue = InVolume;
			UpdatedControlBusMixStage.Value.AttackTime = 0.01f;
			UpdatedControlBusMixStage.Value.ReleaseTime = 0.01f;

			TArray<FSoundControlBusMixStage> UpdatedMixStageArray;
			UpdatedMixStageArray.Add(UpdatedControlBusMixStage);

			UAudioModulationStatics::UpdateMix(AudioWorld, ControlBusMix, UpdatedMixStageArray);
		}
	}
}

void UDCGameSettings::SetHeadphoneModeEnabled(bool bEnabled)
{
	if (CanModifyHeadphoneModeEnabled())
	{
		static IConsoleVariable* BinauralSpatializationDisabledCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.DisableBinauralSpatialization"));
		if (BinauralSpatializationDisabledCVar)
		{
			BinauralSpatializationDisabledCVar->Set(!bEnabled, ECVF_SetByGameSetting);

			if (bUseHeadphoneMode != bEnabled)
			{
				bUseHeadphoneMode = bEnabled;
				SaveSettings();
			}
		}
	}
}

bool UDCGameSettings::CanModifyHeadphoneModeEnabled() const
{
	static IConsoleVariable* BinauralSpatializationDisabledCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.DisableBinauralSpatialization"));
	const bool bHRTFOptionAvailable = BinauralSpatializationDisabledCVar && ((BinauralSpatializationDisabledCVar->GetFlags() & EConsoleVariableFlags::ECVF_SetByMask) <= EConsoleVariableFlags::ECVF_SetByGameSetting);

	return bHRTFOptionAvailable;
}

void UDCGameSettings::SetHDRAudioModeEnabled(bool bEnabled)
{
	bUseHDRAudioMode = bEnabled;

	if (GEngine)
	{
		if (const UWorld* World = GEngine->GetCurrentPlayWorld())
		{
			if (UDCAudioMixEffectsSubsystem* DCAudioMixEffectsSubsystem = World->GetSubsystem<UDCAudioMixEffectsSubsystem>())
			{
				DCAudioMixEffectsSubsystem->ApplyDynamicRangeEffectsChains(bEnabled);
			}
		}
	}
}

void UDCGameSettings::SetAllowAudioInBackgroundSetting(EAllowBackgroundAudioSetting NewValue)
{
	AllowAudioInBackground = NewValue;
	ApplyAudioSettings();
}

void UDCGameSettings::ApplyAudioSettings()
{
	ULocalPlayer* LP = GetTypedOuter<ULocalPlayer>();
	if (LP && LP->IsPrimaryPlayer())
	{
		FApp::SetUnfocusedVolumeMultiplier((AllowAudioInBackground != EAllowBackgroundAudioSetting::Off) ? 1.0f : 0.0f);
	}
}

#pragma endregion


#pragma region Display

void UDCGameSettings::SetDisplayGamma(float InGamma)
{
	DisplayGamma = InGamma;
	ApplyDisplayGamma();
}

void UDCGameSettings::ApplyDisplayGamma()
{
	if (GEngine)
	{
		GEngine->DisplayGamma = DisplayGamma;
	}
}

#pragma endregion


#pragma region Input

void UDCGameSettings::AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, UDCLocalPlayer* LocalPlayer)
{
	if (MappingName == NAME_None)
	{
		return;
	}

	CustomKeyboardConfig.Add(MappingName, NewKey);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->AddPlayerMappedKey(MappingName, NewKey);
	}
}

void UDCGameSettings::ResetKeybindingToDefault(const FName MappingName, UDCLocalPlayer* LocalPlayer)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->RemovePlayerMappedKey(MappingName);
	}
}

void UDCGameSettings::ResetKeybindingsToDefault(UDCLocalPlayer* LocalPlayer)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->RemoveAllPlayerMappedKeys();
	}
}

#pragma endregion


UDCGameSettings* UDCGameSettings::Get()
{
	return GEngine ? CastChecked<UDCGameSettings>(GEngine->GetGameUserSettings()) : nullptr;
}
