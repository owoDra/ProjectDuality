// Copyright (C) 2023 owoDra

#include "DCCharacterPlayableComponent.h"

#include "Development/DCLogChannels.h"
#include "GameplayTag/DCTags_Input.h"
#include "Input/DCInputComponent.h"
#include "Input/DCData_InputConfig.h"
#include "Character/DCCharacter.h"
#include "Character/DCData_Character.h"
#include "Ability/DCAbilitySystemComponent.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCCharacterPlayableComponent)


void UDCCharacterPlayableComponent::InitializePlayerInput(UEnhancedInputLocalPlayerSubsystem* Subsystem, UInputComponent* PlayerInputComponent, UDCData_InputConfig* InputConfig)
{
	check(Subsystem);
	check(PlayerInputComponent);
	check(InputConfig);

	Subsystem->ClearAllMappings();

	auto* DCIC{ CastChecked<UDCInputComponent>(PlayerInputComponent) };
	DCIC->AddInputMappings(InputConfig, Subsystem);

	TArray<uint32> BindHandles;
	DCIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

	DCIC->BindNativeAction(InputConfig, TAG_Input_Move_KM, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
	DCIC->BindNativeAction(InputConfig, TAG_Input_Move_Pad, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
	DCIC->BindNativeAction(InputConfig, TAG_Input_Look_KM, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
	DCIC->BindNativeAction(InputConfig, TAG_Input_Look_Pad, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
}

void UDCCharacterPlayableComponent::OnCharacterDataApplied(const UDCData_Character* CharacterData)
{
	check(CharacterData);

	const auto* Pawn{ GetOwner<APawn>() };
	if (!Pawn)
	{
		UE_LOG(LogDCCharacter, Error, TEXT("[%s]: Owner が無効、または Pawn から派生するクラスではありません"), *GetNameSafe(this));
		return;
	}

	if (!(Pawn->IsPlayerControlled() && Pawn->IsLocallyControlled()))
	{
		UE_LOG(LogDCCharacter, Warning, TEXT("[%s]: Bot によってコントロールされているキャラクターに DCCharacterPlayableComponent がついています"), *GetNameSafe(this));
		return;
	}

	const auto* PlayerController{ Pawn->GetController<APlayerController>() };
	if (!PlayerController)
	{
		UE_LOG(LogDCCharacter, Error, TEXT("[%s]: PlayerController が無効です"), *GetNameSafe(this));
		return;
	}

	const auto* LocalPlayer{ PlayerController->GetLocalPlayer() };
	if (!LocalPlayer)
	{
		UE_LOG(LogDCCharacter, Error, TEXT("[%s]: LocalPlayer が無効です"), *GetNameSafe(this));
		return;
	}

	auto* Subsystem{ LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() };
	auto InputComponent{ Pawn->InputComponent };
	auto InputConfig{ CharacterData->InputConfig };

	InitializePlayerInput(Subsystem, InputComponent, InputConfig);
}


void UDCCharacterPlayableComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const auto* DCCharacter{ GetOwner<ADCCharacter>() })
	{
		if (auto* DCASC{ DCCharacter->GetDCAbilitySystemComponent()})
		{
			DCASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void UDCCharacterPlayableComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const auto* DCCharacter{ GetOwner<ADCCharacter>() })
	{
		if (auto* DCASC{ DCCharacter->GetDCAbilitySystemComponent() })
		{
			DCASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UDCCharacterPlayableComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn{ GetOwner<APawn>() })
	{
		const FVector2D Value{ InputActionValue.Get<FVector2D>() };
		const FRotator MovementRotation{ 0.0f, Pawn->GetControlRotation().Yaw, 0.0f };

		if (Value.X != 0.0f)
		{
			const auto MovementDirection{ MovementRotation.RotateVector(FVector::RightVector) };
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const auto MovementDirection{ MovementRotation.RotateVector(FVector::ForwardVector) };
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UDCCharacterPlayableComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn{ GetOwner<APawn>() })
	{
		const FVector2D Value{ InputActionValue.Get<FVector2D>() };

		if (Value.X != 0.0f)
		{
			Pawn->AddControllerYawInput(Value.X);
		}

		if (Value.Y != 0.0f)
		{
			Pawn->AddControllerPitchInput(Value.Y);
		}
	}
}

void UDCCharacterPlayableComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn{ GetOwner<APawn>() })
	{
		const float DeltaTime{ GetWorld()->GetDeltaSeconds() };

		const FVector2D Value{ InputActionValue.Get<FVector2D>() };

		if (Value.X != 0.0f)
		{
			Pawn->AddControllerYawInput(Value.X * DCPlayableDefaults::GamepadLookYawRate * DeltaTime);
		}

		if (Value.Y != 0.0f)
		{
			Pawn->AddControllerPitchInput(Value.Y * DCPlayableDefaults::GamepadLookPitchRate * DeltaTime);
		}
	}
}


UDCCharacterPlayableComponent* UDCCharacterPlayableComponent::FindCharacterPlayableComponent(const APawn* Pawn)
{
	return (Pawn ? Pawn->FindComponentByClass<UDCCharacterPlayableComponent>() : nullptr);
}
