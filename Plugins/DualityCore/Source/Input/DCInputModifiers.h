// Copyright (C) 2023 owoDra

#pragma once

#include "InputModifiers.h"

#include "DCInputModifiers.generated.h"


/** 
 * DCGameSettings の感度設定を参照して入力の強度をスケーリングする InputModifier
 */
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Settings Driven Sensitivity"))
class UDCInputModifierSensitivity : public UInputModifier
{
public:
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};


/**
 * デッドゾーンを設定するゲームパッドのスティック
 */
UENUM()
enum class EDeadzoneStick : uint8
{
	MoveStick = 0,
	LookStick = 1,
};


/**
　* DCGameSettings のデッドゾーン設定を参照して入力の強度をスケーリングする InputModifier
　*/
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Settings Driven Dead Zone"))
class UDCInputModifierDeadZone : public UInputModifier
{
public:
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
	virtual FLinearColor GetVisualizationColor_Implementation(FInputActionValue SampleValue, FInputActionValue FinalValue) const override;

public:
	//
	// デッドゾーンタイプ
	// (Radial, Axial)
	//
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings", Config)
	EDeadZoneType Type{ EDeadZoneType::Radial };
	
	//
	// 閾値の上限
	//
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings", Config)
	float UpperThreshold{ 1.0f };

	//
	// 度のスティックに対して設定を参照するか
	//
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings", Config)
	EDeadzoneStick DeadzoneStick{ EDeadzoneStick::MoveStick };
};


/**
　* DCGameSettings の反転設定を参照して入力方向を反転する InputModifier
　*/
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Settings Driven Aim Inversion"))
class UDCInputModifierAimInversion : public UInputModifier
{
public:
	GENERATED_BODY()
	
protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;	
};
