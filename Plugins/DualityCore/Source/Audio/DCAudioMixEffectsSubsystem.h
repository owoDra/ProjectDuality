// Copyright (C) 2023 owoDra

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "DCAudioMixEffectsSubsystem.generated.h"

class USoundSubmix;
class USoundControlBus;
class USoundControlBusMix;
class USoundEffectSubmixPreset;


USTRUCT()
struct FDCAudioSubmixEffectsChain
{
public:
	GENERATED_BODY()

public:
	//
	// SubmixEffectsChain の上書きを適用するサブミックス
	//
	UPROPERTY(Transient)
	TObjectPtr<USoundSubmix> Submix{ nullptr };

	//
	// 適用する SubmixEffectsChain
	//
	UPROPERTY(Transient)
	TArray<TObjectPtr<USoundEffectSubmixPreset>> SubmixEffectChain;
};

/**
 * ユーザーのゲーム設定を ControlBusMix に適用するためのサブシステム
 * 
 * ゲーム設定に応じて DCAudioSettings で定義した HDR/LDR AudioSubmixEffectsChain を適用する
 */
UCLASS()
class DUALITYCORE_API UDCAudioMixEffectsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	//virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	/** 
	 * HDR または LDR Audio を適用する
	 */
	void ApplyDynamicRangeEffectsChains(bool bHDRAudio);
	
protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	/**
	 * ロード画面の表示が変更されたときに呼び出される
	 * (表示、非表示)
	 */
	void OnLoadingScreenStatusChanged(bool bShowingLoadingScreen);

	/**
	 * ロード画面の表示に応じて ControlBusMix を適用する
	 */
	void ApplyOrRemoveLoadingScreenMix(bool bWantsLoadingScreenMix);

	//
	// デフォルトの ControlBusMix
	// DCAudioSettings で定義される
	//
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> DefaultBaseMix{ nullptr };

	//
	// ロード画面用の ControlBusMix
	// DCAudioSettings で定義される
	//
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> LoadingScreenMix{ nullptr };

	//
	// ユーザー用の ControlBusMix
	// DCAudioSettings で定義される
	//
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> UserMix{ nullptr };

	//
	// マスター音量の ControlBusMix
	// DCAudioSettings で定義され
	//
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> OverallControlBus{ nullptr };

	//
	// BGM音量の ControlBusMix
	// DCAudioSettings で定義され
	//
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> MusicControlBus{ nullptr };

	//
	// SFX音量の ControlBusMix
	// DCAudioSettings で定義され
	//
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> SoundFXControlBus{ nullptr };

	//
	// HDR Audio 時に適用される AudioSubmixEffectsChain
	// DCAudioSettings で定義される
	//
	UPROPERTY(Transient)
	TArray<FDCAudioSubmixEffectsChain> HDRSubmixEffectChain;

	//
	// LDR Audio 時に適用される AudioSubmixEffectsChain
	// DCAudioSettings で定義される
	//
	UPROPERTY(Transient)
	TArray<FDCAudioSubmixEffectsChain> LDRSubmixEffectChain;

	bool bAppliedLoadingScreenMix{ false };
};
