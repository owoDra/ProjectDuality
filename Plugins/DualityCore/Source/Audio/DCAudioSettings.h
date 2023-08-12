// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DeveloperSettings.h"

#include "DCAudioSettings.generated.h"

class UObject;
class USoundEffectSubmixPreset;
class USoundSubmix;

USTRUCT()
struct DUALITYCORE_API FDCSubmixEffectChainMap
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/Engine.SoundSubmix"))
	TSoftObjectPtr<USoundSubmix> Submix{ nullptr };

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/Engine.SoundEffectSubmixPreset"))
	TArray<TSoftObjectPtr<USoundEffectSubmixPreset>> SubmixEffectChain;
};


/**
 * Audio に関係する ControlBusMix などを定義するデベロッパー設定
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "DCAudioSettings"))
class DUALITYCORE_API UDCAudioSettings : public UDeveloperSettings
{
public:
	GENERATED_BODY()

public:
	//
	// デフォルトの ControlBusMix
	//
	UPROPERTY(config, EditAnywhere, Category = MixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBusMix"))
	FSoftObjectPath DefaultControlBusMix;

	//
	// ロード画面用の ControlBusMix
	//
	UPROPERTY(config, EditAnywhere, Category = MixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBusMix"))
	FSoftObjectPath LoadingScreenControlBusMix;

	//
	// ユーザー用の ControlBusMix
	//
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBusMix"))
	FSoftObjectPath UserSettingsControlBusMix;

	//
	// マスター音量の ControlBusMix
	//
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath OverallVolumeControlBus;

	//
	// BGM音量の ControlBusMix
	//
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath MusicVolumeControlBus;

	//
	// SFX音量の ControlBusMix
	//
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath SoundFXVolumeControlBus;

	//
	// HDR Audio 時に適用される AudioSubmixEffectsChain
	//
	UPROPERTY(config, EditAnywhere, Category = EffectSettings)
	TArray<FDCSubmixEffectChainMap> HDRAudioSubmixEffectChain;
	
	//
	// LDR Audio 時に適用される AudioSubmixEffectsChain
	//
	UPROPERTY(config, EditAnywhere, Category = EffectSettings)
	TArray<FDCSubmixEffectChainMap> LDRAudioSubmixEffectChain;
};
