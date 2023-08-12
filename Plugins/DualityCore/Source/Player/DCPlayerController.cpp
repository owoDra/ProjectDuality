// Copyright (C) 2023 owoDra

#include "DCPlayerController.h"

#include "DCLocalPlayer.h"
#include "Ability/DCAbilitySystemComponent.h"
#include "Character/DCCharacter.h"
#include "GameSetting/DCGameSettings.h"

#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCPlayerController)


void ADCPlayerController::BeginPlay()
{
	auto* Settings{ UDCGameSettings::Get() };
	Settings->OnSettingApplied.AddUObject(this, &ThisClass::OnSettingsChanged);

	OnSettingsChanged(Settings);
}

void ADCPlayerController::UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId)
{
	if (bForceFeedbackEnabled)
	{
		InputInterface->SetForceFeedbackChannelValues(ControllerId, ForceFeedbackValues);
	}

	InputInterface->SetForceFeedbackChannelValues(ControllerId, FForceFeedbackValues());
}

void ADCPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (auto* DCASC{ GetDCAbilitySystemComponent() })
	{
		DCASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ADCPlayerController::OnSettingsChanged(UDCGameSettings* Settings)
{
	bForceFeedbackEnabled = Settings->GetForceFeedbackEnabled();
}


ADCPlayerState* ADCPlayerController::GetDCPlayerState() const
{
	return CastChecked<ADCPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

ADCHUD* ADCPlayerController::GetDCHUD() const
{
	return CastChecked<ADCHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

ADCCharacter* ADCPlayerController::GetOwningDCCharacter() const
{
	return CastChecked<ADCCharacter>(GetPawn(), ECastCheckedType::NullAllowed);
}

UDCAbilitySystemComponent* ADCPlayerController::GetDCAbilitySystemComponent() const
{
	const auto* DCCHR{ GetOwningDCCharacter() };
	return (DCCHR ? DCCHR->GetDCAbilitySystemComponent() : nullptr);
}

