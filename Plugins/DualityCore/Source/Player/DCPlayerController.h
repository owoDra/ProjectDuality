// Copyright (C) 2023 owoDra

#pragma once

#include "GameFramework/PlayerController.h"

#include "DCPlayerController.generated.h"

class ADCPlayerState;
class ADCHUD;
class ADCCharacter;
class UDCAbilitySystemComponent;

/**
 * このプロジェクトで使用される基本 PlayerController クラス。
 */
UCLASS(Config = Game)
class DUALITYCORE_API ADCPlayerController : public APlayerController
{
public:
	GENERATED_BODY()
public:
	ADCPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
	}

protected:
	virtual void BeginPlay() override;
	virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	/**
	 * ゲーム設定が変更されたことを知らせる
	 */
	UFUNCTION()
	void OnSettingsChanged(UDCGameSettings* Settings);

public:
	/**
	 * 所有する DCPlayerState を取得する
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	ADCPlayerState* GetDCPlayerState() const;

	/**
	 * 所有する DCHUD を取得する
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	ADCHUD* GetDCHUD() const;

	/**
	 * 所有する DCCharacter を取得する
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	ADCCharacter* GetOwningDCCharacter() const;

	/**
	 * 所有 DCCharacter の DCAbilitySystemComponent を取得する
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	UDCAbilitySystemComponent* GetDCAbilitySystemComponent() const;
};

