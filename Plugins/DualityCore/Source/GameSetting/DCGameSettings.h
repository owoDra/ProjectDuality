// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/GameUserSettings.h"

#include "Delegates/Delegate.h"

#include "DCGameSettings.generated.h"

class UDCLocalPlayer;
class USoundControlBus;
class USoundControlBusMix;


/**
 *  バックグラウンドサウンドタイプ
 */
UENUM(BlueprintType)
enum class EAllowBackgroundAudioSetting : uint8
{
	Off,
	AllSounds,

	Num UMETA(Hidden)
};


/**
 * 色覚特性タイプ
 */
UENUM(BlueprintType)
enum class EColorBlindMode : uint8
{
	Off,
	Deuteranope,
	Protanope,
	Tritanope
};


/**
 * このゲームの設定を保存するためのクラス
 */
UCLASS()
class UDCGameSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UDCGameSettings();

public:
	virtual void BeginDestroy() override;

	virtual void LoadSettings(bool bForceReload) override;

	virtual void ResetToCurrentSettings() override;
	virtual void ApplyNonResolutionSettings() override;

public:
	DECLARE_EVENT_OneParam(UDCGameSettings, FOnSettingChangedEvent, UDCGameSettings* Settings);
	FOnSettingChangedEvent OnSettingApplied;


	////////////////////////////////////////
	// コントロール設定

	// ===== 反転設定 =====
private:
	// 縦軸の反転
	UPROPERTY(Config) bool bInvertVerticalAxis{ false };

	// 横軸の反転
	UPROPERTY(Config) bool bInvertHorizontalAxis{ false };

public:
	/**
	 *  縦軸の反転
	 */
	UFUNCTION() bool GetInvertVerticalAxis() const { return bInvertVerticalAxis; }
	UFUNCTION() void SetInvertVerticalAxis(bool NewValue) { bInvertVerticalAxis = NewValue; }

	/**
	 *  横軸の反転
	 */
	UFUNCTION() bool GetInvertHorizontalAxis() const { return bInvertHorizontalAxis; }
	UFUNCTION() void SetInvertHorizontalAxis(bool NewValue) { bInvertHorizontalAxis = NewValue; }


	// ===== 感度設定 =====
private:
	// 視点移動の感度
	UPROPERTY(Config) double Sensitivity{ 1.0 };

public:
	/**
	 *  視点移動の感度
	 */
	UFUNCTION() double GetSensitivity() const { return Sensitivity; }
	UFUNCTION() void SetSensitivity(double NewValue) { Sensitivity = NewValue; }


	// ===== 振動設定 =====
private:
	// 振動を有効にするかどうか
	UPROPERTY(Config) bool bForceFeedbackEnabled{ true };

public:
	/**
	 *  振動設定
	 */
	UFUNCTION() bool GetForceFeedbackEnabled() const { return bForceFeedbackEnabled; }
	UFUNCTION() void SetForceFeedbackEnabled(bool NewValue) { bForceFeedbackEnabled = NewValue; }


	// ===== デッドゾーン設定 =====
private:
	// 移動用のスティックのデッドゾーン
	UPROPERTY() float GamepadMoveStickDeadZone;

	// 視点操作用のスティックのデッドゾーン
	UPROPERTY() float GamepadLookStickDeadZone;

public:
	/**
	 *  移動用のスティックのデッドゾーン
	 */
	UFUNCTION() float GetGamepadMoveStickDeadZone() const { return GamepadMoveStickDeadZone; }
	UFUNCTION() void SetGamepadMoveStickDeadZone(float NewValue) { GamepadMoveStickDeadZone = NewValue; }

	/**
	 *  視点操作用のスティックのデッドゾーン
	 */
	UFUNCTION() float GetGamepadLookStickDeadZone() const { return GamepadLookStickDeadZone; }
	UFUNCTION() void SetGamepadLookStickDeadZone(float NewValue) { GamepadLookStickDeadZone = NewValue; }



	////////////////////////////////////////
	// ゲームプレイ設定

	// ===== 言語設定 =====
private:
	// 現在適応されている保留中の言語
	UPROPERTY(Transient) FString PendingCulture;

	// デフォルトの言語にリセットするかどうか。
	// True ならばリセットする。
	bool bResetToDefaultCulture{ false };

public:
	/**
	 *  現在適応されている保留中の言語
	 */
	const FString& GetPendingCulture() const { return PendingCulture; }
	void SetPendingCulture(const FString& NewCulture);

	/**
	 *  言語が変更されたときに実行される
	 */
	void OnCultureChanged();

	/**
	 *  現在保留中の言語を削除する
	 */
	void ClearPendingCulture();

	/**
	 *  現在デフォルトの言語を使用しているかどうか
	 */
	bool IsUsingDefaultCulture() const;

	/**
	 *  デフォルトの言語にリセットする
	 */
	void ResetToDefaultCulture();

	/**
	 *  現在デフォルトの言語を使用しているかどうか
	 */
	bool ShouldResetToDefaultCulture() const { return bResetToDefaultCulture; }

	/**
	 *  言語設定を適応する
	 */
	void ApplyCultureSettings();

	/**
	 *  適応中の言語を設定中の言語に戻す
	 */
	void ResetCultureToCurrentSettings();


	// ===== 色覚特性設定 =====
private:
	// 色覚特性の種類
	UPROPERTY(Config) EColorBlindMode ColorBlindMode { EColorBlindMode::Off };

	// 色覚特性の強さ
	UPROPERTY(Config) int32 ColorBlindStrength { 10 };

public:
	/**
	 *  色覚特性の種類
	 */
	UFUNCTION() EColorBlindMode GetColorBlindMode() const { return ColorBlindMode; }
	UFUNCTION() void SetColorBlindMode(EColorBlindMode InMode);

	/**
	 *  色覚特性の強さ
	 */
	UFUNCTION() int32 GetColorBlindStrength() const { return ColorBlindStrength; }
	UFUNCTION() void SetColorBlindStrength(int32 InColorBlindStrength);


	/////////////////////////////////////////////
	// ディスプレイ設定
protected:
	/**
	 *  ディスプレイの明るさを適応
	 */
	void ApplyDisplayGamma();

private:
	// ディスプレイの明るさ
	UPROPERTY(Config) float DisplayGamma{ 2.2 };

public:
	UFUNCTION() float GetDisplayGamma() const { return DisplayGamma; }
	UFUNCTION() void SetDisplayGamma(float InGamma);


	/////////////////////////////////////////////
	// オーディオデバイス設定
private:
	UPROPERTY(Transient) TMap<FName, USoundControlBus*> ControlBusMap;
	UPROPERTY(Transient) USoundControlBusMix* ControlBusMix{ nullptr };
	UPROPERTY(Transient) bool bSoundControlBusMixLoaded{ false };

public:
	/**
	 *  サウンドのコントロールバスを読み込む
	 */
	void LoadUserControlBusMix();


private:
	// 音声出力デバイスのID
	UPROPERTY(Config) FString AudioOutputDeviceId;

public:
	UFUNCTION() FString GetAudioOutputDeviceId() const { return AudioOutputDeviceId; }
	UFUNCTION() void SetAudioOutputDeviceId(const FString& InAudioOutputDeviceId);


public:
	// 立体音響の仕様を求めているかどうか
	UPROPERTY(Transient) bool bDesiredHeadphoneMode;

private:
	// 立体音響を利用するかどうか
	UPROPERTY(config) bool bUseHeadphoneMode{ false };

public:
	UFUNCTION() bool IsHeadphoneModeEnabled() const { return bUseHeadphoneMode; }
	UFUNCTION() void SetHeadphoneModeEnabled(bool bEnabled);
	UFUNCTION() bool CanModifyHeadphoneModeEnabled() const;


private:
	// HDR オーディオを使用するかどうか
	UPROPERTY(config) bool bUseHDRAudioMode{ false };

public:
	UFUNCTION() bool IsHDRAudioModeEnabled() const { return bUseHDRAudioMode; }
	UFUNCTION() void SetHDRAudioModeEnabled(bool bEnabled);


private:
	// バックグランドで音楽を再生し続けるかどうか
	UPROPERTY(config) EAllowBackgroundAudioSetting AllowAudioInBackground = EAllowBackgroundAudioSetting::Off;

public:
	UFUNCTION() EAllowBackgroundAudioSetting GetAllowAudioInBackgroundSetting() const { return AllowAudioInBackground; }
	UFUNCTION() void SetAllowAudioInBackgroundSetting(EAllowBackgroundAudioSetting NewValue);

	void ApplyAudioSettings();


private:
	// それぞれのサウンドボリューム
	UPROPERTY(config) float OverallVolume{ 1.0 };
	UPROPERTY(config) float MusicVolume{ 1.0 };
	UPROPERTY(config) float SoundFXVolume{ 1.0 };

public:
	UFUNCTION() float GetOverallVolume() const { return OverallVolume; }
	UFUNCTION() void SetOverallVolume(float InVolume);

	UFUNCTION() float GetMusicVolume() const { return MusicVolume; }
	UFUNCTION() void SetMusicVolume(float InVolume);

	UFUNCTION() float GetSoundFXVolume() const { return SoundFXVolume; }
	UFUNCTION() void SetSoundFXVolume(float InVolume);

protected:
	/**
	 * ControlBusの音量を変える
	 */
	void SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume);




	/////////////////////////////////////////////
	// キーバインド設定
private:
	UPROPERTY(Config) TMap<FName, FKey> CustomKeyboardConfig;

public:
	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }


public:
	/**
	 * プレイヤーがゲーム設定で変更したキーバインドを適用する
	 */
	void AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, UDCLocalPlayer* LocalPlayer);

	/**
	 * プレイヤーがゲーム設定で変更したキーバインドをデフォルトに戻す
	 */
	void ResetKeybindingToDefault(const FName MappingName, UDCLocalPlayer* LocalPlayer);

	/**
	 * プレイヤーがゲーム設定で変更した全てのキーバインドをデフォルトに戻す
	 */
	void ResetKeybindingsToDefault(UDCLocalPlayer* LocalPlayer);


	/////////////////////////////////////////////
	//	イベント
private:
	/**
	 *  アプリの状態が変わったとき
	 */
	void ReapplyThingsDueToPossibleDeviceProfileChange();

	FDelegateHandle OnApplicationActivationStateChangedHandle;

	//
	// 音響出力デバイスが変更されたときに呼ばれるデリゲート
	//
public:
	DECLARE_EVENT_OneParam(UDCGameSettings, FAudioDeviceChanged, const FString& /*DeviceId*/);
	FAudioDeviceChanged OnAudioOutputDeviceChanged;


	/////////////////////////////////////////////
	//	ユーティリティ
public:
	/**
	 *  このクラスのインスタンスが存在する場合にそれ取得する
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameSettings", meta = (DisplayName = "GetGameSettings"))
	static UDCGameSettings* Get();
};
