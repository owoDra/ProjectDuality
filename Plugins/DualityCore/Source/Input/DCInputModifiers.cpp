// Copyright (C) 2023 owoDra

#include "DCInputModifiers.h"

#include "Development/DCLogChannels.h"
#include "GameSetting/DCGameSettings.h"

#include "EnhancedPlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Logging/LogMacros.h"
#include "Math/Vector2D.h"
#include "Misc/AssertionMacros.h"
#include "Templates/Casts.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCInputModifiers)


#pragma region Settings Driven Sensitivity

FInputActionValue UDCInputModifierSensitivity::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	// 設定が有効かどうか検証

	const auto* Settings{ UDCGameSettings::Get() };
	if (!Settings)
	{
		UE_LOG(LogDCInput, Warning, TEXT("DCGameSettings が有効ではありません。ProjectSettings->GameUserSettingsClass を変更してください"));
		return CurrentValue;
	}

	// 設定の値を適用する

	const auto Sensitivity{ Settings->GetSensitivity() };

	return CurrentValue.Get<FVector>() * Sensitivity;
}

#pragma endregion


#pragma region Settings Driven Dead Zone

FInputActionValue UDCInputModifierDeadZone::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	// 設定が有効かどうか検証

	const auto* Settings{ UDCGameSettings::Get() };
	if (!Settings)
	{
		UE_LOG(LogDCInput, Warning, TEXT("DCGameSettings が有効ではありません。ProjectSettings->GameUserSettingsClass を変更してください"));
		return CurrentValue;
	}

	// 設定の値を適用する

	const auto LowerThreshold{
		(DeadzoneStick == EDeadzoneStick::MoveStick) ? 
		FMath::Clamp(Settings->GetGamepadMoveStickDeadZone(), 0.0f, 1.0f) :
		FMath::Clamp(Settings->GetGamepadLookStickDeadZone(), 0.0f, 1.0f)
	};
	
	/**
	 * デッドゾーンでトリミングした値を +/- 1 の範囲に変換するラムダ関数
	 */
	auto DeadZoneLambda = [LowerThreshold, this](const float AxisVal)
	{
		return FMath::Min(1.f, (FMath::Max(0.f, FMath::Abs(AxisVal) - LowerThreshold) / (UpperThreshold - LowerThreshold))) * FMath::Sign(AxisVal);
	};

	auto NewValue{ CurrentValue.Get<FVector>() };

	switch (Type)
	{
	case EDeadZoneType::Axial:
		NewValue.X = DeadZoneLambda(NewValue.X);
		NewValue.Y = DeadZoneLambda(NewValue.Y);
		NewValue.Z = DeadZoneLambda(NewValue.Z);
		break;

	case EDeadZoneType::Radial:
		switch (CurrentValue.GetValueType())
		{
		case EInputActionValueType::Axis3D:
			NewValue = NewValue.GetSafeNormal() * DeadZoneLambda(NewValue.Size());
			break;

		case EInputActionValueType::Axis2D:
			NewValue = NewValue.GetSafeNormal2D() * DeadZoneLambda(NewValue.Size2D());
			break;

		default:
			NewValue.X = DeadZoneLambda(NewValue.X);
			break;
		}
		break;
	}

	return NewValue;
}

FLinearColor UDCInputModifierDeadZone::GetVisualizationColor_Implementation(FInputActionValue SampleValue, FInputActionValue FinalValue) const
{
	if (FinalValue.GetValueType() == EInputActionValueType::Boolean || FinalValue.GetValueType() == EInputActionValueType::Axis1D)
	{
		return FLinearColor((FinalValue.Get<float>() == 0.f) ? 1.f : 0.f, 0.f, 0.f);
	}
	return FLinearColor(((FinalValue.Get<FVector2D>().X == 0.f) ? 0.5f : 0.f) + ((FinalValue.Get<FVector2D>().Y == 0.f) ? 0.5f : 0.f), 0.f, 0.f);
}

#pragma endregion


#pragma region Settings Driven Aim Inversion

FInputActionValue UDCInputModifierAimInversion::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	// 設定が有効かどうか検証

	const auto* Settings{ UDCGameSettings::Get() };
	if (!Settings)
	{
		UE_LOG(LogDCInput, Warning, TEXT("DCGameSettings が有効ではありません。ProjectSettings->GameUserSettingsClass を変更してください"));
		return CurrentValue;
	}

	// 設定の値を適用する

	FVector NewValue = CurrentValue.Get<FVector>();
	
	if (Settings->GetInvertVerticalAxis())
	{
		NewValue.Y *= -1.0f;
	}
	
	if (Settings->GetInvertHorizontalAxis())
	{
		NewValue.X *= -1.0f;
	}
	
	return NewValue;
}

#pragma endregion
