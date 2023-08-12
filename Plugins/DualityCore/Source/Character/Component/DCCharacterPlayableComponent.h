// Copyright (C) 2023 owoDra

#pragma once

#include "Components/ActorComponent.h"

#include "Character/DCCharacterComponentInterface.h"

#include "DCCharacterPlayableComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputComponent;
class UDCData_InputConfig;
class UDCData_Character;
struct FGameplayTag;
struct FInputActionValue;


/**
 *	キャラクター操作のデフォルト値の定数
 */
namespace DCPlayableDefaults
{
	static constexpr auto GamepadLookYawRate{ 300.0f };
	static constexpr auto GamepadLookPitchRate{ 165.0f };
};


/**
 *	キャラクターがプレイヤーによって操作可能にするためのコンポーネント
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class DUALITYCORE_API UDCCharacterPlayableComponent 
	: public UActorComponent
	, public IDCCharacterComponentInterface
{
public:
	GENERATED_BODY()
public:
	UDCCharacterPlayableComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
	}

protected:
	/**
	 * プレイヤーの入力によるキャラクターのイベントとの紐づけを行う
	 */
	virtual void InitializePlayerInput(UEnhancedInputLocalPlayerSubsystem* Subsystem, UInputComponent* PlayerInputComponent, UDCData_InputConfig* InputConfig);

public:
	virtual void OnCharacterDataApplied(const UDCData_Character* CharacterData) override;


protected:
	/**
	 * プレイヤーのアビリティ関連の押し込み入力を受けた時に実行
	 */
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);

	/**
	 * プレイヤーのアビリティ関連のリリース入力を受けた時に実行
	 */
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	/**
	 * プレイヤーの移動関連の入力を受けた時に実行
	 */
	void Input_Move(const FInputActionValue& InputActionValue);

	/**
	 * プレイヤーのマウスによる視点移動関連の入力を受けた時に実行
	 */
	void Input_LookMouse(const FInputActionValue& InputActionValue);

	/**
	 * プレイヤーのゲームパッドによる視点移動関連の入力を受けた時に実行
	 */
	void Input_LookStick(const FInputActionValue& InputActionValue);


public:
	/**
	 * 指定した Pawn が所有する DCCharacterPlayableComponent を取得する
	 */
	UFUNCTION(BlueprintPure, Category = "Character")
	static UDCCharacterPlayableComponent* FindCharacterPlayableComponent(const APawn* Pawn);
};
