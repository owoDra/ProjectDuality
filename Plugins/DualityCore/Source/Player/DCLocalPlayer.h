// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/LocalPlayer.h"

#include "DCLocalPlayer.generated.h"

class UInputMappingContext;


/**
 * Player そのものが持つ情報(ゲーム設定、デバイス設定、入力設定、など)を保有するクラス
 */
UCLASS()
class DUALITYCORE_API UDCLocalPlayer : public ULocalPlayer
{
public:
	GENERATED_BODY()
public:
	UDCLocalPlayer() {}

public:
	virtual void PostInitProperties() override;

private:
	//
	// プレイヤーの入力設定
	//
	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;

protected:
	/**
	 * Player のオーディオデバイスが変わったときに呼び出される
	 */
	void OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId);
	
	/**
	 * Player のオーディオデバイスの変更が完了した時
	 */
	UFUNCTION()
	void OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult);
};
